#include <cmath>
#include <assert.h>

#include "Box.h"

bool Box::intersectLocal( const ray& r, isect& i ) const
{
	// YOUR CODE HERE:
    // Add box intersection code here.
	// it currently ignores all boxes and just returns false.
	// http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm

	double tNear = -std::numeric_limits<double>::max();
	double tFar = std::numeric_limits<double>::max();

	const vec3f rayDirection = r.getDirection();
	const vec3f rayPosition = r.getPosition();

	for (int i = 0; i < 3; i++) {
		if (rayDirection[i] == 0) {
			if (rayDirection[i] < -0.5 || rayDirection[i] > 0.5) {
				return false;
			}
		}

		double t1 = (-0.5 - rayPosition[i]) / rayDirection[i];
		double t2 = (0.5 - rayPosition[i]) / rayDirection[i];

		if (t1 > t2) {
			double temp = t1;
			t1 = t2;
			t2 = temp;
		}

		if (t1 > tNear) {
			tNear = t1;
		}

		if (t2 < tFar) {
			tFar = t2;
		}

		if (tNear > tFar || tFar < 0) {
			return false;
		}
	}

	i.obj = this;
	i.setT(tNear);

	return true;
}
