#include "ContactChecker.h"
#include <math.h>


// Checks for the contact of the two boxes with an edge length of 2 each. Both boxes are 
// origin centered. The local coordinate system of both boxes is handed over.
bool ContactChecker::CheckForContact(const D3DXMATRIX& coordinateSystemA,const D3DXMATRIX& coordinateSystemB)
{
	// First we have to setup the transformation from B into A. This is A^-1 B
	D3DXMATRIX inverseA;
	D3DXMatrixInverse(&inverseA, NULL, &coordinateSystemA);
	D3DXMatrixMultiply(&m_relativeCoordinateSystemB, &coordinateSystemB, &inverseA);
	// As we do not need the translational part of the matrix and as the rest of the matric is a pure
	// rotation we can also use the cheap inversion in form of the transpose in our case.
	D3DXMatrixTranspose(&m_relativeCoordinateSystemBInverted,  &m_relativeCoordinateSystemB);

	D3DXVECTOR3 cubeAAxis[3];
	cubeAAxis[0] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	cubeAAxis[1] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	cubeAAxis[2] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	for (int i = 0; i < 3; ++i)
	{
		if (isSeparatingAxis(cubeAAxis[i]))
		{
			return false;
		}
	}

	D3DXVECTOR3 cubeBAxis[3];
	
	for (int i = 0; i < 3; ++i)
	{
		cubeBAxis[i] = D3DXVECTOR3(m_relativeCoordinateSystemB.m[i][0], m_relativeCoordinateSystemB.m[i][1], m_relativeCoordinateSystemB.m[i][2]);

		if (isSeparatingAxis(cubeBAxis[i]))
		{
			return false;
		}
	}

	D3DXVECTOR3 candidate;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			D3DXVec3Cross(&candidate, &cubeAAxis[i], &cubeBAxis[j]);

			if (isSeparatingAxis(candidate))
			{
				return false;
			}
		}
	}
	return true;
}

// This method checks if an indicated axis is a separating one.
bool ContactChecker::isSeparatingAxis(const D3DXVECTOR3& candidate)
{
	D3DXVECTOR3 vector(m_relativeCoordinateSystemB.m[3][0], m_relativeCoordinateSystemB.m[3][1], m_relativeCoordinateSystemB.m[3][2]);

	float leftSide = fabsf(D3DXVec3Dot(&vector, &candidate));

	float rightSideA = fabsf(candidate.x) + fabsf(candidate.y) + fabsf(candidate.z);

	D3DXVECTOR3 localDirection;

	D3DXVec3TransformNormal(&localDirection, &candidate, &m_relativeCoordinateSystemBInverted);

	float rightSideB = fabsf(localDirection.x) + fabsf(localDirection.y) + fabsf(localDirection.z);

	return (leftSide > rightSideA + rightSideB);
}