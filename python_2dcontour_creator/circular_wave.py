import numpy as np

class CircularWave:
    def __init__(self, x0=0.0, y0=0.0, freq_r=1.0, freq_t=1.0, phase=0.0,
                 wave_type='harmonic', amplitude=1.0, start_time=0.0):
        self.x0, self.y0 = x0, y0
        self.freq_r = freq_r
        self.freq_t = freq_t
        self.phase = phase
        self.wave_type = wave_type
        self.amplitude = amplitude
        self.start_time = start_time

    def evaluate(self, x, y, t=0.0):
        if t < self.start_time:
            return np.zeros_like(x)
        
        # Funcion of the form A * cos(2*pi*(f_r*r - f_t*t) + phase)
        # Use relative time since trigger
        dt = t - self.start_time
        r = np.sqrt((x - self.x0)**2 + (y - self.y0)**2)
        arg = 2 * np.pi * (self.freq_r * r - self.freq_t * dt) + self.phase

        if self.wave_type == 'harmonic':
            return self.amplitude * np.cos(arg)
        elif self.wave_type == 'pulse':
            mask = (arg > -np.pi/2) & (arg < np.pi/2)
            return self.amplitude * np.cos(arg) * mask
        return np.zeros_like(x)




