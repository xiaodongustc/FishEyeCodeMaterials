#pragma once
#include <OpencvCommon.h>


class Rotation
{
public:
	//minAngle and maxAngle is radian
	Rotation(double minAngle = 0.0, double maxAngle = CV_2PI)
	{
		assert(maxAngle >= minAngle);
		axis = _randomAxis();
		double ratio = rand() / double(RAND_MAX);
		angle = minAngle + ratio * (maxAngle - minAngle);

		R = _axisAngleToMatrix(axis, angle);
	}

	Rotation(const cv::Vec3d &ax, double radian)
	{
		axis[0] = ax[0]; axis[1] = ax[1]; axis[2] = ax[2];
		angle = radian;
		R = _axisAngleToMatrix(ax, radian);
	}

	cv::Vec3d axis;
	double angle;
	cv::Mat R;

private:
	cv::Vec3d _randomAxis()
	{
		double theta = CV_2PI * (rand() / double(RAND_MAX));
		double phi = CV_PI * (rand() / double(RAND_MAX));
		/*double theta = CV_PI * 0.5;
		double phi = CV_PI * 0.5;*/
		return cv::Vec3d(sin(phi)*cos(theta), sin(phi)*sin(theta), cos(phi));
	}

	//axis-angle to rotation matrix formula follow
	//http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/
	cv::Mat _axisAngleToMatrix(const cv::Vec3d &ax, double &radian)
	{
		double s = sin(radian);
		double c_1 = cos(radian) - 1;

		cv::Mat M = cv::Mat(3, 3, CV_64FC1);
		double *pR = reinterpret_cast<double *>(M.data);
		pR[0] = 1 - c_1 * (ax[0] * ax[0] - 1);
		pR[1] = -ax[2] * s - c_1 * ax[0] * ax[1];
		pR[2] = ax[1] * s - c_1 * ax[0] * ax[2];
		pR[3] = ax[2] * s - c_1 * ax[0] * ax[1];
		pR[4] = 1 - c_1 * (ax[1] * ax[1] - 1);
		pR[5] = -ax[0] * s - c_1 * ax[1] * ax[2];
		pR[6] = -ax[1] * s - c_1 * ax[0] * ax[2];
		pR[7] = ax[0] * s - c_1 * ax[1] * ax[2];
		pR[8] = 1 - c_1 * (ax[2] * ax[2] - 1);
		return M;
	}
};

inline cv::Point3d RotatePoint(const cv::Point3d &srcPt, const Rotation &rot)
{
	double *pR = reinterpret_cast<double *>(rot.R.data);
	return cv::Point3d(pR[0] * srcPt.x + pR[1] * srcPt.y + pR[2] * srcPt.z,
					   pR[3] * srcPt.x + pR[4] * srcPt.y + pR[5] * srcPt.z,
					   pR[6] * srcPt.x + pR[7] * srcPt.y + pR[8] * srcPt.z);
}


