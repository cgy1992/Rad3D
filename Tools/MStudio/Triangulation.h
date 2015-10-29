#pragma once

#include "MStudioEntry.h"

#define MAX_VERTEX 10000

struct VERTEX
{
	double x;
	double y;
	double z;
};

struct TRIANGLE
{
	int     vv0;
	int     vv1;
	int     vv2;
};


int FvsTriangleInCircle(double xp, double yp, double x1, double y1, double x2, double y2, double x3, double y3, double *xc,double *yc, double *r)
{
	int  Ret;
	double eps;
	double m1;
	double m2;
	double mx1;
	double mx2;
	double my1;
	double my2;
	double dx;
	double dy;
	double rsqr;
	double drsqr;
	eps = 0.000001;
	Ret = 0;
	if ( fabs(y1 - y2) < eps && fabs(y2 - y3) < eps )
	{
		printf("INCIRCUM - F - Points are coincident !\n");
		return Ret;
	}
	if ( fabs(y2 - y1) < eps )
	{
		m2 = -(x3 - x2) / (y3 - y2);
		mx2 = (x2 + x3) / 2;
		my2 = (y2 + y3) / 2;
		*xc = (x2 + x1) / 2;
		*yc = m2 * (*xc - mx2) + my2;
	}
	else if ( fabs(y3 - y2) < eps )
	{
		m1 = -(x2 - x1) / (y2 - y1);
		mx1 = (x1 + x2) / 2;
		my1 = (y1 + y2) / 2;
		*xc = (x3 + x2) / 2;
		*yc = m1 * ((*xc) - mx1) + my1;
	}
	else
	{
		m1 = -(x2 - x1) / (y2 - y1);
		m2 = -(x3 - x2) / (y3 - y2);
		mx1 = (x1 + x2) / 2;
		mx2 = (x2 + x3) / 2;
		my1 = (y1 + y2) / 2;
		my2 = (y2 + y3) / 2;
		*xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
		*yc = m1 * ((*xc) - mx1) + my1;
	}
	dx = x2 - (*xc);
	dy = y2 - (*yc);
	rsqr = dx * dx + dy * dy;
	*r = sqrt(rsqr);
	dx = xp - (*xc);
	dy = yp - (*yc);
	drsqr = dx * dx + dy * dy;
	if ( drsqr <= rsqr )
		return 1;
	else
		return 0;
}


int FvsTriangulation(int nvert, VERTEX Vertex[], TRIANGLE Triangle[])
{
	int Complete[MAX_VERTEX];
	long Edges[3][MAX_VERTEX];
	long Nedge;
	long xmin;
	long xmax;
	long ymin;
	long ymax;
	long xmid;
	long ymid;
	double dx;
	double dy;
	double dmax;
	int i;
	int j;
	int k;
	int ntri;
	double xc;
	double yc;
	double r;
	int  inc;
	for(i=1; i <= 200; i++)
		Complete[i] = 0;
	xmin = Vertex[1].x;
	ymin = Vertex[1].y;
	xmax = xmin;
	ymax = ymin;
	for ( i = 2; i <= nvert; i++)
	{
		if (Vertex[i].x < xmin)      xmin = Vertex[i].x;
		if (Vertex[i].x > xmax)      xmax = Vertex[i].x;
		if (Vertex[i].y < ymin)      ymin = Vertex[i].y;
		if (Vertex[i].y > ymax)      ymax = Vertex[i].y;
	}
	dx = xmax - xmin;
	dy = ymax - ymin;
	if ( dx > dy )
		dmax = dx;
	else
		dmax = dy;
	xmid = (xmax + xmin) / 2;
	ymid = (ymax + ymin) / 2;
	Vertex[nvert + 1].x = xmid - 2 * (long)dmax;
	Vertex[nvert + 1].y = ymid - (long)dmax;
	Vertex[nvert + 2].x = xmid;
	Vertex[nvert + 2].y = ymid + 2 * (long)dmax;
	Vertex[nvert + 3].x = xmid + 2 * (long)dmax;
	Vertex[nvert + 3].y = ymid - (long)dmax;
	Triangle[1].vv0 = nvert + 1;
	Triangle[1].vv1 = nvert + 2;
	Triangle[1].vv2 = nvert + 3;
	Complete[1] = 0;
	ntri = 1;
	for ( i = 1; i <= nvert; i++)
	{
		Nedge = 0;
		j = 0;
		do
		{
			j = j + 1;
			if ( Complete[j] != 1 )
			{
				inc = FvsTriangleInCircle(Vertex[i].x, Vertex[i].y, Vertex[Triangle[j].vv0].x,Vertex[Triangle[j].vv0].y, Vertex[Triangle[j].vv1].x, Vertex[Triangle[j].vv1].y, Vertex[Triangle[j].vv2].x, Vertex[Triangle[j].vv2].y,&xc, &yc, &r);
				if ( inc==1 )
				{
					Edges[1][Nedge + 1] = Triangle[j].vv0;
					Edges[2][Nedge + 1] = Triangle[j].vv1;
					Edges[1][Nedge + 2] = Triangle[j].vv1;
					Edges[2][Nedge + 2] = Triangle[j].vv2;
					Edges[1][Nedge + 3] = Triangle[j].vv2;
					Edges[2][Nedge + 3] = Triangle[j].vv0;
					Nedge = Nedge + 3;
					Triangle[j].vv0 = Triangle[ntri].vv0;
					Triangle[j].vv1 = Triangle[ntri].vv1;
					Triangle[j].vv2 = Triangle[ntri].vv2;
					Complete[j] = Complete[ntri];
					j = j - 1;
					ntri = ntri - 1;
				}
			}
		}while(j < ntri);
		for ( j = 1; j <= Nedge - 1; j++)
		{
			if ( !(Edges[1][j] == 0) && !(Edges[2][j] == 0) )
			{
				for ( k = j + 1; k <= Nedge; k++)
				{
					if ( !(Edges[1][k] == 0 ) && !(Edges[2][k] == 0) )
					{
						if ( Edges[1][j] == Edges[2][k] )
						{
							if ( Edges[2][j] == Edges[1][k] )
							{
								Edges[1][j] = 0;
								Edges[2][j] = 0;
								Edges[1][k] = 0;
								Edges[2][k] = 0;
							}
						}
					}
				}
			}
		}
		for ( j = 1; j <= Nedge; j++)
		{
			if ( !(Edges[1][j] == 0) && !(Edges[2][j] == 0 ) )
			{
				ntri = ntri + 1;
				Triangle[ntri].vv0 = Edges[1][j];
				Triangle[ntri].vv1 = Edges[2][j];
				Triangle[ntri].vv2 = i;
				Complete[ntri] =0;
			}
		}
	} /* end of For */

	i = 0;
	do
	{
		i = i + 1;
		if ( Triangle[i].vv0 > nvert || Triangle[i].vv1 > nvert || Triangle[i].vv2 > nvert )
		{
			Triangle[i].vv0 = Triangle[ntri].vv0;
			Triangle[i].vv1 = Triangle[ntri].vv1;
			Triangle[i].vv2 = Triangle[ntri].vv2;
			i = i - 1;
			ntri = ntri - 1;
		}
	}while( i < ntri );
	return ntri;
}