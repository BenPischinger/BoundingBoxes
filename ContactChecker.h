#pragma once

#include "DXUT/DXUT.h"
#include "DXUT/DXUTmisc.h"

// This class ist checking the contact situation of two boxes that have an edge length of 
// one. This is the case in our example.
class ContactChecker
{
public:
	
	// Checks for the contact of the two boxes with an edge length of 2 each. Both boxes are 
	// origin centered. The local coordinate system of both boxes is handed over.
	bool CheckForContact(const D3DXMATRIX& coordinateSystemA,const D3DXMATRIX& coordinateSystemB); 

private:
	// Describes the bounding box B in the relative coordinate system of A.
	D3DXMATRIX m_relativeCoordinateSystemB;
	// Inverted system of the above.
	D3DXMATRIX m_relativeCoordinateSystemBInverted;
	
	// This method checks if an indicated axis is a separating one.
	bool isSeparatingAxis(const D3DXVECTOR3& candidate);
};

