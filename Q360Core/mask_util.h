#ifndef __MASK_UTIL_H__
#define __MASK_UTIL_H__

inline bool GetLineParam(double x1, double y1, double x2, double y2, double &a, double &b)
{
	if ((x2 - x1) == 0){
		a = 0;
	}
	else if ((y2 - y1) == 0)
		a = 0;
	else
		a = (y2 - y1) / (x2 - x1);

	b = y1 - (a)*x1;

	return true;
}
inline int calcLinePoint(double a, double b, double x)
{
	double y = (a*x + b);
	//int yy = (int)y;
	//QLOGD("TEST", "a(%lf), b(%lf), y(%lf), yy(%d)\n", y, yy);
	return (int)y;
}
inline int calcLinePointY(double a, double b, double y)
{
	double x;
	if (a == 0) x = 0;
	else x = (y-b)/a;
	return (int)x;
}
inline void calcOrthoLineParam(double x, double y, double a, double b, double &ret_a, double &ret_b)
{
	ret_a = -1 / a;
	ret_b = y - (ret_a * x);
}

#endif //__MASK_UTIL_H__