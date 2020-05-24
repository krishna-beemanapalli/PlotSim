#ifndef _MYSIMULATOR_HH_
#define _MYSIMULATOR_HH_

#include <cstdint>

#include "simulator.hh"

namespace brown {

/**
 * @brief Simulator subclass with direct hardware control.
 *
 * You have no further restrictions on the implementation of this class beyond
 * inheriting class `brown::Simulator`.
 */
class MySimulator: public Simulator {
public:
    MySimulator(): Simulator() {};

    /**
     * @brief Pulse generation logic for plotting.
     * @note You must override this pure virtual method.
     *
     * This method will be invoked repeatedly and indefinitely by the
     * simulator. You are recommended to handle one point at a time.
     * You can invoke `this->setpin` to generate pulses.
     */
    void hardwareLoop();

private:
	short x = 0, xp = 0, y = 0, yp = 0, dx = 0, dy = 0;
	bool down = 0, dirx, diry, pulx, puly;
	int row_n = 0;
	float pulses_per_revolution = 800;
	float cm_per_revolution = 1;
	uint64_t acceleration_time = 150;
	float cmpulse = cm_per_revolution / pulses_per_revolution;
	int np_x = 0, np_y = 0;
	bool longer, longer_pulse, shorter_pulse; 
	uint64_t clk = 0;
	float pulse_start = 300;
	float slew_rate = 10000;
	uint64_t timec = acceleration_time * 10000000;
	double pulse_rate;
	int calc_short, calc_long;

	int compute_p(short x) {
		return (0.01*x)/cmpulse;
	}

	double acceleration(double pulse_step, int i, uint64_t iterations, uint64_t clk1, bool fh) {
		double new_step;
		if (i <= (iterations / 2)) {
			return slew_rate - (slew_rate - pulse_start) * exp(-4 * (double)(clk1 - pulse_step) / (double)timec);
		}
		else {
			if (!fh) {
				new_step = (uint64_t)((double)(clk1 - pulse_step) * 2);
				fh = true;
			}
			return slew_rate - (slew_rate - pulse_start) * exp(4 * (double)((clk1 - pulse_step) - new_step) / (double)timec);
		}
	}
};

} // namespace brown

#endif // _MYSIMULATOR_HH_
