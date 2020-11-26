#include "secp256k1class.h"

void Secp256k1::FillS()
{
	memset(S, 0u, sizeof(S));
	memcpy(S, &G, sizeof(G));

	for (uint64_t i = 1; i < 256u; i++)
	{
		S[i] = Double(*(S + i - 1u));
	}
}

point Secp256k1::Add(point A, point B)
{
	point C;
	memset(&C, 0u, sizeof(C));
	uint256_t pin;
	memcpy(pin, p, sizeof(pin));

#define r0 &r[0]
#define r1 &r[4]
#define r2 &r[8]
#define r3 &r[12]
#define r4 &r[16]

	uint64_t r[20];
	uint64_t m[4];
	uint64_t xp[4];
	uint64_t yp[4];
	uint64_t xq[4];
	uint64_t yq[4];

	memset(r, 0, sizeof(r));
	memset(m, 0, sizeof(m));

	if (more_u256(A.x, B.x) == 0)
	{
		memcpy(xq, A.x, sizeof(xq));
		memcpy(yq, A.y, sizeof(yq));
		memcpy(xp, B.x, sizeof(xp));
		memcpy(yp, B.y, sizeof(yp));
	}
	else
	{
		memcpy(xq, B.x, sizeof(xq));
		memcpy(yq, B.y, sizeof(yq));
		memcpy(xp, A.x, sizeof(xp));
		memcpy(yp, A.y, sizeof(yp));
	}

	sub_u256(xp, xq, r0);
	inv_u256(r0, pin, r1);
	sub_u256(pin, yq, r2);
	add_u256_mod(yp, r2, pin, r3);
	mul_u256_mod(r1, r3, pin, m);

	memset(r, 0, sizeof(r));

	mul_u256_mod(m, m, pin, r0);
	add_u256_mod(xq, xp, pin, r1);
	sub_u256(pin, r1, r3);
	add_u256_mod(r0, r3, pin, C.x);

	memset(r, 0, sizeof(r));

	sub_u256(pin, xp, r0);
	add_u256_mod(C.x, r0, pin, r1);
	mul_u256_mod(r1, m, pin, r2);
	add_u256_mod(r2, yp, pin, r3);
	sub_u256(pin, r3, C.y);

#undef r0
#undef r1
#undef r2
#undef r3
#undef r4
	return C;
}

point Secp256k1::Double(point A)
{
	point B;
	memset(&B, 0u, sizeof(B));
	uint256_t pin;
	memcpy(pin, p, sizeof(pin));

#define r0 &r[0]
#define r1 &r[4]
#define r2 &r[8]
#define r3 &r[12]
#define r4 &r[16]

	uint64_t r[20];
	uint64_t m[4];

	memset(r, 0, sizeof(r));

	mul_u256_mod(A.x, A.x, pin, r0);
	add_u256_mod(r0, r0, pin, r1);
	add_u256_mod(r0, r1, pin, r1);
	add_u256_mod(A.y, A.y, pin, r2);
	inv_u256(r2, pin, r3);
	mul_u256_mod(r3, r1, pin, m);

	memset(r, 0, sizeof(r));

	mul_u256_mod(m, m, pin, r0);
	add_u256_mod(A.x, A.x, pin, r1);
	sub_u256(pin, r1, r3);
	add_u256_mod(r0, r3, pin, B.x);

	memset(r, 0, sizeof(r));

	sub_u256(pin, A.x, r0);
	add_u256_mod(B.x, r0, pin, r1);
	mul_u256_mod(r1, m, pin, r2);
	add_u256_mod(r2, A.y, pin, r3);
	sub_u256(pin, r3, B.y);

#undef r0
#undef r1
#undef r2
#undef r3
#undef r4
	return B;
}

point Secp256k1::PublicKey(uint256_t prik)
{
	point pubk;
	memset(&pubk, 0u, sizeof(pubk));
	for (uint64_t i = 0u; i < 4u; i++)
	{
		for (uint64_t j = 0u; j < 64u; j++)
		{
			if (((prik[i] >> j) & 1u) == 1u)
			{
				uint64_t shift = i * 64u + j;
				if ((equalzero_u256(pubk.x) == 1) && (equalzero_u256(pubk.y) == 1))
				{
					memcpy(&pubk, S + shift, sizeof(pubk));
				}
				else
				{
					pubk = Add(pubk, *(S + shift));
				}
			}
		}
	}
	return pubk;
}

Secp256k1::Secp256k1()
{
	FillS();
}

void Secp256k1::SetPrivateKey(uint64_t* k)
{
	memcpy(privatekey, k, sizeof(privatekey));
	publickeyp = PublicKey(privatekey);
}

void Secp256k1::SetIncrement(uint64_t* i)
{
	memcpy(inc, i, sizeof(inc));
	incp = PublicKey(inc);
}

void Secp256k1::Increment()
{
	add_u256(privatekey, inc, privatekey);
	if ((equal_u256(publickeyp.x, incp.x) == 1) && (equal_u256(publickeyp.y, incp.y) == 1))
	{
		publickeyp = Double(publickeyp);
	}
	else
	{
		publickeyp = Add(publickeyp, incp);
	}
}

void Secp256k1::Get(uint64_t* k, uint64_t* px, uint64_t* py)
{
	memcpy(k, privatekey, sizeof(privatekey));
	memcpy(px, publickeyp.x, sizeof(publickeyp.x));
	memcpy(py, publickeyp.y, sizeof(publickeyp.y));
}