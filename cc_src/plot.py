import matplotlib.pyplot as plt
from matplotlib import animation, rc
import seaborn as sns
import numpy as np
import sys
import os
#print(os.getcwd())
sys.path.append(os.getcwd())
import heat

def simulate(heatobj, nsim):
    for _ in range(nsim):
        heatobj.step()
    return heatobj


def circle(center, radius):
    data = np.linspace(0, 2*np.pi, 1000)
    X = radius*np.cos(data) + center[0]
    Y = radius*np.sin(data) + center[1]
    plt.plot(X, Y)

if __name__ == "__main__":
    mc = heat.MeshCircle(100, 100, 1e-3, (-1, 1), (-1, 1))

    for i in range(1000):
        mc.step()
        print(sum(np.array(mc)))

    fig, ax = plt.subplots()
    gca = plt.axes()
    gca.set_aspect('equal')

    def init():
        return [ax]

    def animate(i):
        global cb
        plt.gcf().clear()
        plt.axes().set_aspect('equal')
        circle([0,0], 1)

        #x_, y_, u_ = invgrid(-1, 1, -1, 1, 20, 20, lambda x, y: x**2 + y**2 <= 1.0**2, u[:, i])
        #c = plt.contourf(x_, y_, u_,  vmin=0, vmax=1)
        mc.step()
        c = plt.contourf(np.linspace(-1, 1, 100), np.linspace(-1, 1, 100), np.array(mc), vmin=0, vmax=1)
        plt.colorbar(c)
        plt.title("t={:.2f}".format((i+1)/1000))
        return c

    print(mc)
    ani = animation.FuncAnimation(fig, animate, init_func=init, frames=range(0, 1000, 20), interval=200, blit=False)
    plt.show()
