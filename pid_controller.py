"""
PID Feedback Controller Implementation
Author: Ekenedilichukwu Emmanuel Nsiadu
Description: Classical continuous control algorithm for precision motor drive 
             and closed-loop feedback systems in robotics.
"""

import time

class PIDController:
    def __init__(self, kp: float, ki: float, kd: float, setpoint: float = 0.0):
        self.kp = kp  # Proportional Gain
        self.ki = ki  # Integral Gain
        self.kd = kd  # Derivative Gain
        self.setpoint = setpoint

        self._prev_error = 0.0
        self._integral = 0.0

    def compute(self, current_value: float, dt: float) -> float:
        """Calculates control output variable based on systemic error."""
        error = self.setpoint - current_value
        
        # Proportional term
        p_term = self.kp * error
        
        # Integral term
        self._integral += error * dt
        i_term = self.ki * self._integral
        
        # Derivative term
        d_term = self.kd * ((error - self._prev_error) / dt) if dt > 0 else 0.0
        
        self._prev_error = error
        
        # Total Control Signal Output
        return p_term + i_term + d_term

    def update_setpoint(self, new_setpoint: float):
        self.setpoint = new_setpoint
        self._integral = 0.0


if __name__ == "__main__":
    # Example: Robot position feedback control loop target = 100 mm
    controller = PIDController(kp=1.2, ki=0.05, kd=0.1, setpoint=100.0)
    current_position = 0.0
    dt = 0.1  # 100ms cycle time

    print(f"Target Setpoint: {controller.setpoint} mm")
    print("-" * 40)

    for step in range(1, 15):
        # Calculate corrective control output
        control_output = controller.compute(current_position, dt)
        
        # Simulate physical system response
        current_position += control_output * 0.2
        
        print(f"Step {step:02d} | Position: {current_position:.2f} mm | Control Output: {control_output:.2f}")
        time.sleep(0.05)
