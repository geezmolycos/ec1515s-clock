import optuna
import numpy as np

import matplotlib.pyplot as plt
import numpy as np

formula = lambda x: 1/(-0.000612217191425404*np.log10((1024/x)-1)+0.0031170331653)

# target temp range: -20~60 C
# ADC range: 401~981
X = np.arange(401, 982, 1, dtype=np.float64)
real = formula(X)

# You probably won't need this if you're embedding things in a tkinter plot...
plt.ion()

fig = plt.figure()
ax = fig.add_subplot(111)
line1, = ax.plot(X, real, 'r-') # Returns a tuple of line objects, thus the comma
line2, = ax.plot(X, real, 'b-') # Returns a tuple of line objects, thus the comma

def error_amount(a, b, c, d):
    approx = a + b*X + c*X*X + d*X*X*X
    return np.sum((approx - real) ** 2)

def objective(trial):
    # a = trial.suggest_float('a', 415, 417)
    # b = trial.suggest_float('b', -0.32, -0.300)
    # c = trial.suggest_float('c', 0.0003, 0.00035)
    # d = trial.suggest_float('d', -0.0000003, 0)
    a = trial.suggest_float('a', 400, 480)
    b = trial.suggest_float('b', -1, 1)
    c = trial.suggest_float('c', -0.1, 0.1)
    d = trial.suggest_float('d', -0.1, 0.1)
    return error_amount(a, b, c, d)

n = 0
def show(study,trial):
    if trial.number % 50 != 0:
        return
    a, b, c, d = [study.best_params[k] for k in 'abcd']
    line1.set_ydata(a + b*X + c*X*X + d*X*X*X)
    fig.canvas.draw()
    fig.canvas.flush_events()
try:
    study = optuna.create_study(sampler=optuna.samplers.CmaEsSampler())
    study.optimize(objective, callbacks=[show])
finally:
    print(study.best_params)  # E.g. {'x': 2.002108042}

    print(error_amount(1, 1, 1, 1))
    print(error_amount(416, -0.31, 0.000338, -0.000000185))
