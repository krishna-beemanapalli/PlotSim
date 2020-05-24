#include <cstdlib>

#include "mysimulator.hh"
#include "ri.hh"

namespace brown {

	void MySimulator::hardwareLoop() {
		down = static_cast<bool>(state[row_n][0]);
		x = state[row_n][1];
		y = state[row_n][2];
		row_n += 1;
		if (down == 0 && x == 0 && y == 0) {
			setpin(clk, false, false, false, false, false);
		}
		else {
			//change in coords
			dx = x - xp;
			dy = y - yp;
			//calculate pulses
			np_x = compute_p(dx);
			np_y = compute_p(dy);
			if (abs(dx) >= abs(dy)) {
				longer = true;
			}
			else
				longer = false;
			//track axis
			short longer_axis = (longer ? abs(np_x) : abs(np_y));
			short shorter_axis = (longer ? abs(np_y) : abs(np_x));

			//pulse loop
			uint64_t iterations = (uint64_t)((double)(abs(longer_axis) - 1) * 100) + 1;

			//track directions
			if (dx >= 0)
				dirx = true;
			else
				dirx = false;
			if (dy >= 0)
				diry = true;
			else
				diry = false;
			
			uint64_t pulse_step = clk;
			bool first_half = false;
			int calc_long1 = -1, calc_short1 = -1;
			
			for (uint64_t i = 0; i < iterations; i++) {
				shorter_pulse = false; longer_pulse = false;
				calc_long = i / 100;
				calc_short = (i * shorter_axis) / (longer_axis) / 100;
				if (calc_long != abs(longer_axis) && calc_long > calc_short1) {
					longer_pulse = true;
					calc_short1 = calc_long;
				}
				if (calc_short != abs(shorter_axis) && calc_short > calc_long1) {
					shorter_pulse = true;
					calc_long1 = calc_short;
				}
				pulx = (longer?longer_pulse : shorter_pulse);
				puly = (longer?shorter_pulse : longer_pulse);
				if (puly == true || pulx == true) {
					this->setpin(clk, pulx, puly, dirx, diry, down);
				}

				//acceleration and deceleration
				double pulse_rate = acceleration(pulse_step, i, iterations, clk, first_half);
				clk += (uint64_t)((double)1000000 / (double)(pulse_rate));
			}
			//update coords
			xp = x;
			yp = y;
		}

	}

} // namespace brown
