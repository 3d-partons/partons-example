import numpy as np
import matplotlib.pyplot as plt
import MatplotlibSettings
from scipy.interpolate import make_interp_spline, BSpline

im = plt.imread("logo_partons.png")

fig, ax = plt.subplots()
ax.plot(range(10))

# Loada data
data = np.loadtxt("GPDs.dat")

xb = data[:500,1]

##############################################################
fwd = make_interp_spline(data[:500,1], data[:500,2], k = 3)(xb)
apf = make_interp_spline(data[:500,1], data[:500,3], k = 3)(xb)
vnn = make_interp_spline(data[:500,1], data[:500,4], k = 3)(xb)

f, (ax1, ax2) = plt.subplots(2, 1, sharex = "all", gridspec_kw = dict(width_ratios = [1], height_ratios = [4, 1]))
plt.subplots_adjust(wspace = 0, hspace = 0)

ax1.text(0.0013, 0.19,  r"\textbf{GK model ($\mu_0=2$ GeV)}", fontsize = 18)
ax1.text(0.0013, 0.16,  r"\textbf{LO evolution}", fontsize = 18)
ax1.text(0.0013, 0.12,  r"\textbf{$\xi = 0.2$}", fontsize = 18)
ax1.text(0.0013, 0.09, r"\textbf{$t=-0.1$ GeV$^2$}", fontsize = 18)
ax1.text(0.0013, 0.06, r"\textbf{$\mu=10$ GeV}", fontsize = 18)
ax1.set(ylabel = r"$xH_{\rm NS}(x,\xi,t,\mu)$")
ax1.set_xlim([0.001, 0.9])
ax1.set_ylim([-0.02, 0.4])
ax1.set_xscale("log")
ax1.plot(xb, fwd, label = r"\textbf{Forward}", color = "black", ls = "--", lw = 1.5)
ax1.plot(xb, apf, color = "red",  label = r"\textbf{APFEL++ {(prel.)}}")
ax1.plot(xb, vnn, color = "blue", label = r"\textbf{Vinnikov}")
ax1.legend()

ax2.set(xlabel = r"\textbf{$x$}")
ax2.set(ylabel = r"\textbf{Ratio}")
ax2.set_xlim([0.001, 0.9])
ax2.set_ylim([0.3, 1.3])
ax2.set_xscale("log")
ax2.plot(xb, fwd/fwd, "k--", lw = 1.5)
ax2.plot(xb, apf/fwd, color = "red")
ax2.plot(xb, vnn/fwd, color = "blue")

newax = f.add_axes([0.8, 0.77, 0.1, 0.1])
newax.imshow(im, alpha = 0.5)
newax.axis('off')

plt.savefig("GPDs_NS_10.pdf")
plt.close()

##############################################################
fwd = make_interp_spline(data[:500,1], data[:500,5], k = 3)(xb)
apf = make_interp_spline(data[:500,1], data[:500,6], k = 3)(xb)
vnn = make_interp_spline(data[:500,1], data[:500,7], k = 3)(xb)

f10 = fwd
v10 = vnn

f, (ax1, ax2) = plt.subplots(2, 1, sharex = "all", gridspec_kw = dict(width_ratios = [1], height_ratios = [4, 1]))
plt.subplots_adjust(wspace = 0, hspace = 0)

ax1.text(0.0013, 0.38, r"\textbf{GK model ($\mu_0=2$ GeV)}", fontsize = 18)
ax1.text(0.0013, 0.32, r"\textbf{LO evolution}", fontsize = 18)
ax1.text(0.0013, 0.24, r"\textbf{$\xi = 0.2$}", fontsize = 18)
ax1.text(0.0013, 0.18, r"\textbf{$t=-0.1$ GeV$^2$}", fontsize = 18)
ax1.text(0.0013, 0.12, r"\textbf{$\mu=10$ GeV}", fontsize = 18)
ax1.set(ylabel = r"$xH_{\rm SG}(x,\xi,t,\mu)$")
ax1.set_xlim([0.001, 0.9])
ax1.set_ylim([-0.02, 0.8])
ax1.set_xscale("log")
ax1.plot(xb, fwd, label = r"\textbf{Forward}", color = "black", ls = "--", lw = 1.5)
ax1.plot(xb, apf, color = "red",  label = r"\textbf{APFEL++ (prel.)}")
ax1.plot(xb, vnn, color = "blue", label = r"\textbf{Vinnikov}")
ax1.legend()

ax2.set(xlabel = r"\textbf{$x$}")
ax2.set(ylabel = r"\textbf{Ratio}")
ax2.set_xlim([0.001, 0.9])
ax2.set_ylim([-0.1, 1.3])
ax2.set_xscale("log")
ax2.plot(xb, fwd/fwd, "k--", lw = 1.5)
ax2.plot(xb, apf/fwd, color = "red")
ax2.plot(xb, vnn/fwd, color = "blue")

newax = f.add_axes([0.8, 0.77, 0.1, 0.1])
newax.imshow(im, alpha = 0.5)
newax.axis('off')

plt.savefig("GPDs_SG_10.pdf")
plt.close()

##############################################################
fwd = make_interp_spline(data[500:1000,1], data[500:1000,2], k = 3)(xb)
apf = make_interp_spline(data[500:1000,1], data[500:1000,3], k = 3)(xb)
vnn = make_interp_spline(data[500:1000,1], data[500:1000,4], k = 3)(xb)

f, (ax1, ax2) = plt.subplots(2, 1, sharex = "all", gridspec_kw = dict(width_ratios = [1], height_ratios = [4, 1]))
plt.subplots_adjust(wspace = 0, hspace = 0)

ax1.text(0.0013, 0.19,  r"\textbf{GK model ($\mu_0=2$ GeV)}", fontsize = 18)
ax1.text(0.0013, 0.16,  r"\textbf{LO evolution}", fontsize = 18)
ax1.text(0.0013, 0.12,  r"\textbf{$\xi = 0.2$}", fontsize = 18)
ax1.text(0.0013, 0.09, r"\textbf{$t=-0.1$ GeV$^2$}", fontsize = 18)
ax1.text(0.0013, 0.06, r"\textbf{$\mu=3$ GeV}", fontsize = 18)
ax1.set(ylabel = r"$xH_{\rm NS}(x,\xi,t,\mu)$")
ax1.set_xlim([0.001, 0.9])
ax1.set_ylim([-0.02, 0.4])
ax1.set_xscale("log")
ax1.plot(xb, fwd, label = r"\textbf{Forward}", color = "black", ls = "--", lw = 1.5)
ax1.plot(xb, apf, color = "red",  label = r"\textbf{APFEL++ {(prel.)}}")
ax1.plot(xb, vnn, color = "blue", label = r"\textbf{Vinnikov}")
ax1.legend()

ax2.set(xlabel = r"\textbf{$x$}")
ax2.set(ylabel = r"\textbf{Ratio}")
ax2.set_xlim([0.001, 0.9])
ax2.set_ylim([0.3, 1.3])
ax2.set_xscale("log")
ax2.plot(xb, fwd/fwd, "k--", lw = 1.5)
ax2.plot(xb, apf/fwd, color = "red")
ax2.plot(xb, vnn/fwd, color = "blue")

newax = f.add_axes([0.8, 0.77, 0.1, 0.1])
newax.imshow(im, alpha = 0.5)
newax.axis('off')

plt.savefig("GPDs_NS_3.pdf")
plt.close()

##############################################################
fwd = make_interp_spline(data[500:1000,1], data[500:1000,5], k = 3)(xb)
apf = make_interp_spline(data[500:1000,1], data[500:1000,6], k = 3)(xb)
vnn = make_interp_spline(data[500:1000,1], data[500:1000,7], k = 3)(xb)

f3 = fwd
v3 = vnn

f, (ax1, ax2) = plt.subplots(2, 1, sharex = "all", gridspec_kw = dict(width_ratios = [1], height_ratios = [4, 1]))
plt.subplots_adjust(wspace = 0, hspace = 0)

ax1.text(0.0013, 0.38, r"\textbf{GK model ($\mu_0=2$ GeV)}", fontsize = 18)
ax1.text(0.0013, 0.32, r"\textbf{LO evolution}", fontsize = 18)
ax1.text(0.0013, 0.24, r"\textbf{$\xi = 0.2$}", fontsize = 18)
ax1.text(0.0013, 0.18, r"\textbf{$t=-0.1$ GeV$^2$}", fontsize = 18)
ax1.text(0.0013, 0.12, r"\textbf{$\mu=3$ GeV}", fontsize = 18)
ax1.set(ylabel = r"$xH_{\rm SG}(x,\xi,t,\mu)$")
ax1.set_xlim([0.001, 0.9])
ax1.set_ylim([-0.02, 0.8])
ax1.set_xscale("log")
ax1.plot(xb, fwd, label = r"\textbf{Forward}", color = "black", ls = "--", lw = 1.5)
ax1.plot(xb, apf, color = "red",  label = r"\textbf{APFEL++ (prel.)}")
ax1.plot(xb, vnn, color = "blue", label = r"\textbf{Vinnikov}")
ax1.legend()

ax2.set(xlabel = r"\textbf{$x$}")
ax2.set(ylabel = r"\textbf{Ratio}")
ax2.set_xlim([0.001, 0.9])
ax2.set_ylim([-0.1, 1.3])
ax2.set_xscale("log")
ax2.plot(xb, fwd/fwd, "k--", lw = 1.5)
ax2.plot(xb, apf/fwd, color = "red")
ax2.plot(xb, vnn/fwd, color = "blue")

newax = f.add_axes([0.8, 0.77, 0.1, 0.1])
newax.imshow(im, alpha = 0.5)
newax.axis('off')

plt.savefig("GPDs_SG_3.pdf")
plt.close()

##############################################################
fwd = make_interp_spline(data[1000:1500,1], data[1000:1500,2], k = 3)(xb)
apf = make_interp_spline(data[1000:1500,1], data[1000:1500,3], k = 3)(xb)
vnn = make_interp_spline(data[1000:1500,1], data[1000:1500,4], k = 3)(xb)

f, (ax1, ax2) = plt.subplots(2, 1, sharex = "all", gridspec_kw = dict(width_ratios = [1], height_ratios = [4, 1]))
plt.subplots_adjust(wspace = 0, hspace = 0)

ax1.text(0.0013, 0.19,  r"\textbf{GK model ($\mu_0=2$ GeV)}", fontsize = 18)
ax1.text(0.0013, 0.16,  r"\textbf{LO evolution}", fontsize = 18)
ax1.text(0.0013, 0.12,  r"\textbf{$\xi = 0.2$}", fontsize = 18)
ax1.text(0.0013, 0.09, r"\textbf{$t=-0.1$ GeV$^2$}", fontsize = 18)
ax1.text(0.0013, 0.06, r"\textbf{$\mu=5$ GeV}", fontsize = 18)
ax1.set(ylabel = r"$xH_{\rm NS}(x,\xi,t,\mu)$")
ax1.set_xlim([0.001, 0.9])
ax1.set_ylim([-0.02, 0.4])
ax1.set_xscale("log")
ax1.plot(xb, fwd, label = r"\textbf{Forward}", color = "black", ls = "--", lw = 1.5)
ax1.plot(xb, apf, color = "red",  label = r"\textbf{APFEL++ {(prel.)}}")
ax1.plot(xb, vnn, color = "blue", label = r"\textbf{Vinnikov}")
ax1.legend()

ax2.set(xlabel = r"\textbf{$x$}")
ax2.set(ylabel = r"\textbf{Ratio}")
ax2.set_xlim([0.001, 0.9])
ax2.set_ylim([0.3, 1.3])
ax2.set_xscale("log")
ax2.plot(xb, fwd/fwd, "k--", lw = 1.5)
ax2.plot(xb, apf/fwd, color = "red")
ax2.plot(xb, vnn/fwd, color = "blue")

newax = f.add_axes([0.8, 0.77, 0.1, 0.1])
newax.imshow(im, alpha = 0.5)
newax.axis('off')

plt.savefig("GPDs_NS_5.pdf")
plt.close()

##############################################################
fwd = make_interp_spline(data[1000:1500,1], data[1000:1500,5], k = 3)(xb)
apf = make_interp_spline(data[1000:1500,1], data[1000:1500,6], k = 3)(xb)
vnn = make_interp_spline(data[1000:1500,1], data[1000:1500,7], k = 3)(xb)

f5 = fwd
v5 = vnn

f, (ax1, ax2) = plt.subplots(2, 1, sharex = "all", gridspec_kw = dict(width_ratios = [1], height_ratios = [4, 1]))
plt.subplots_adjust(wspace = 0, hspace = 0)

ax1.text(0.0013, 0.38, r"\textbf{GK model ($\mu_0=2$ GeV)}", fontsize = 18)
ax1.text(0.0013, 0.32, r"\textbf{LO evolution}", fontsize = 18)
ax1.text(0.0013, 0.24, r"\textbf{$\xi = 0.2$}", fontsize = 18)
ax1.text(0.0013, 0.18, r"\textbf{$t=-0.1$ GeV$^2$}", fontsize = 18)
ax1.text(0.0013, 0.12, r"\textbf{$\mu=5$ GeV}", fontsize = 18)
ax1.set(ylabel = r"$xH_{\rm SG}(x,\xi,t,\mu)$")
ax1.set_xlim([0.001, 0.9])
ax1.set_ylim([-0.02, 0.8])
ax1.set_xscale("log")
ax1.plot(xb, fwd, label = r"\textbf{Forward}", color = "black", ls = "--", lw = 1.5)
ax1.plot(xb, apf, color = "red",  label = r"\textbf{APFEL++ (prel.)}")
ax1.plot(xb, vnn, color = "blue", label = r"\textbf{Vinnikov}")
ax1.legend()

ax2.set(xlabel = r"\textbf{$x$}")
ax2.set(ylabel = r"\textbf{Ratio}")
ax2.set_xlim([0.001, 0.9])
ax2.set_ylim([-0.1, 1.3])
ax2.set_xscale("log")
ax2.plot(xb, fwd/fwd, "k--", lw = 1.5)
ax2.plot(xb, apf/fwd, color = "red")
ax2.plot(xb, vnn/fwd, color = "blue")

newax = f.add_axes([0.8, 0.77, 0.1, 0.1])
newax.imshow(im, alpha = 0.5)
newax.axis('off')

plt.savefig("GPDs_SG_5.pdf")
plt.close()

##############################################################
fwd = make_interp_spline(data[1500:2000,1], data[1500:2000,2], k = 3)(xb)
apf = make_interp_spline(data[1500:2000,1], data[1500:2000,3], k = 3)(xb)
vnn = make_interp_spline(data[1500:2000,1], data[1500:2000,4], k = 3)(xb)

f, (ax1, ax2) = plt.subplots(2, 1, sharex = "all", gridspec_kw = dict(width_ratios = [1], height_ratios = [4, 1]))
plt.subplots_adjust(wspace = 0, hspace = 0)

ax1.text(0.0013, 0.19,  r"\textbf{GK model ($\mu_0=2$ GeV)}", fontsize = 18)
ax1.text(0.0013, 0.16,  r"\textbf{LO evolution}", fontsize = 18)
ax1.text(0.0013, 0.12,  r"\textbf{$\xi = 0.2$}", fontsize = 18)
ax1.text(0.0013, 0.09, r"\textbf{$t=-0.1$ GeV$^2$}", fontsize = 18)
ax1.text(0.0013, 0.06, r"\textbf{$\mu=2$ GeV}", fontsize = 18)
ax1.set(ylabel = r"$xH_{\rm NS}(x,\xi,t,\mu)$")
ax1.set_xlim([0.001, 0.9])
ax1.set_ylim([-0.02, 0.4])
ax1.set_xscale("log")
ax1.plot(xb, fwd, label = r"\textbf{Forward}", color = "black", ls = "--", lw = 1.5)
ax1.plot(xb, apf, color = "red",  label = r"\textbf{APFEL++ {(prel.)}}")
ax1.plot(xb, vnn, color = "blue", label = r"\textbf{Vinnikov}")
ax1.legend()

ax2.set(xlabel = r"\textbf{$x$}")
ax2.set(ylabel = r"\textbf{Ratio}")
ax2.set_xlim([0.001, 0.9])
ax2.set_ylim([0.3, 1.3])
ax2.set_xscale("log")
ax2.plot(xb, fwd/fwd, "k--", lw = 1.5)
ax2.plot(xb, apf/fwd, color = "red")
ax2.plot(xb, vnn/fwd, color = "blue")

newax = f.add_axes([0.8, 0.77, 0.1, 0.1])
newax.imshow(im, alpha = 0.5)
newax.axis('off')

plt.savefig("GPDs_NS_2.pdf")
plt.close()

##############################################################
fwd = make_interp_spline(data[1500:2000,1], data[1500:2000,5], k = 3)(xb)
apf = make_interp_spline(data[1500:2000,1], data[1500:2000,6], k = 3)(xb)
vnn = make_interp_spline(data[1500:2000,1], data[1500:2000,7], k = 3)(xb)

f2 = fwd
v2 = vnn

f, (ax1, ax2) = plt.subplots(2, 1, sharex = "all", gridspec_kw = dict(width_ratios = [1], height_ratios = [4, 1]))
plt.subplots_adjust(wspace = 0, hspace = 0)

ax1.text(0.0013, 0.38, r"\textbf{GK model ($\mu_0=2$ GeV)}", fontsize = 18)
ax1.text(0.0013, 0.32, r"\textbf{LO evolution}", fontsize = 18)
ax1.text(0.0013, 0.24, r"\textbf{$\xi = 0.2$}", fontsize = 18)
ax1.text(0.0013, 0.18, r"\textbf{$t=-0.1$ GeV$^2$}", fontsize = 18)
ax1.text(0.0013, 0.12, r"\textbf{$\mu=2$ GeV}", fontsize = 18)
ax1.set(ylabel = r"$xH_{\rm SG}(x,\xi,t,\mu)$")
ax1.set_xlim([0.001, 0.9])
ax1.set_ylim([-0.02, 0.8])
ax1.set_xscale("log")
ax1.plot(xb, fwd, label = r"\textbf{Forward}", color = "black", ls = "--", lw = 1.5)
ax1.plot(xb, apf, color = "red",  label = r"\textbf{APFEL++ (prel.)}")
ax1.plot(xb, vnn, color = "blue", label = r"\textbf{Vinnikov}")
ax1.legend()

ax2.set(xlabel = r"\textbf{$x$}")
ax2.set(ylabel = r"\textbf{Ratio}")
ax2.set_xlim([0.001, 0.9])
ax2.set_ylim([-0.1, 1.3])
ax2.set_xscale("log")
ax2.plot(xb, fwd/fwd, "k--", lw = 1.5)
ax2.plot(xb, apf/fwd, color = "red")
ax2.plot(xb, vnn/fwd, color = "blue")

newax = f.add_axes([0.8, 0.77, 0.1, 0.1])
newax.imshow(im, alpha = 0.5)
newax.axis('off')

plt.savefig("GPDs_SG_2.pdf")
plt.close()

##############################################################
fwd = make_interp_spline(data[2000:2500,1], data[2000:2500,2], k = 3)(xb)
apf = make_interp_spline(data[2000:2500,1], data[2000:2500,3], k = 3)(xb)
vnn = make_interp_spline(data[2000:2500,1], data[2000:2500,4], k = 3)(xb)

f, (ax1, ax2) = plt.subplots(2, 1, sharex = "all", gridspec_kw = dict(width_ratios = [1], height_ratios = [4, 1]))
plt.subplots_adjust(wspace = 0, hspace = 0)

ax1.text(0.0013, 0.19,  r"\textbf{GK model ($\mu_0=2$ GeV)}", fontsize = 18)
ax1.text(0.0013, 0.16,  r"\textbf{LO evolution}", fontsize = 18)
ax1.text(0.0013, 0.12,  r"\textbf{$\xi = 0.2$}", fontsize = 18)
ax1.text(0.0013, 0.09, r"\textbf{$t=-0.1$ GeV$^2$}", fontsize = 18)
ax1.text(0.0013, 0.06, r"\textbf{$\mu=2.5$ GeV}", fontsize = 18)
ax1.set(ylabel = r"$xH_{\rm NS}(x,\xi,t,\mu)$")
ax1.set_xlim([0.001, 0.9])
ax1.set_ylim([-0.02, 0.4])
ax1.set_xscale("log")
ax1.plot(xb, fwd, label = r"\textbf{Forward}", color = "black", ls = "--", lw = 1.5)
ax1.plot(xb, apf, color = "red",  label = r"\textbf{APFEL++ {(prel.)}}")
ax1.plot(xb, vnn, color = "blue", label = r"\textbf{Vinnikov}")
ax1.legend()

ax2.set(xlabel = r"\textbf{$x$}")
ax2.set(ylabel = r"\textbf{Ratio}")
ax2.set_xlim([0.001, 0.9])
ax2.set_ylim([0.3, 1.3])
ax2.set_xscale("log")
ax2.plot(xb, fwd/fwd, "k--", lw = 1.5)
ax2.plot(xb, apf/fwd, color = "red")
ax2.plot(xb, vnn/fwd, color = "blue")

newax = f.add_axes([0.8, 0.77, 0.1, 0.1])
newax.imshow(im, alpha = 0.5)
newax.axis('off')

plt.savefig("GPDs_NS_2.5.pdf")
plt.close()

##############################################################
fwd = make_interp_spline(data[2000:2500,1], data[2000:2500,5], k = 3)(xb)
apf = make_interp_spline(data[2000:2500,1], data[2000:2500,6], k = 3)(xb)
vnn = make_interp_spline(data[2000:2500,1], data[2000:2500,7], k = 3)(xb)

f2p5 = fwd
v2p5 = vnn

f, (ax1, ax2) = plt.subplots(2, 1, sharex = "all", gridspec_kw = dict(width_ratios = [1], height_ratios = [4, 1]))
plt.subplots_adjust(wspace = 0, hspace = 0)

ax1.text(0.0013, 0.38, r"\textbf{GK model ($\mu_0=2$ GeV)}", fontsize = 18)
ax1.text(0.0013, 0.32, r"\textbf{LO evolution}", fontsize = 18)
ax1.text(0.0013, 0.24, r"\textbf{$\xi = 0.2$}", fontsize = 18)
ax1.text(0.0013, 0.18, r"\textbf{$t=-0.1$ GeV$^2$}", fontsize = 18)
ax1.text(0.0013, 0.12, r"\textbf{$\mu=2.5$ GeV}", fontsize = 18)
ax1.set(ylabel = r"$xH_{\rm SG}(x,\xi,t,\mu)$")
ax1.set_xlim([0.001, 0.9])
ax1.set_ylim([-0.02, 0.8])
ax1.set_xscale("log")
ax1.plot(xb, fwd, label = r"\textbf{Forward}", color = "black", ls = "--", lw = 1.5)
ax1.plot(xb, apf, color = "red",  label = r"\textbf{APFEL++ (prel.)}")
ax1.plot(xb, vnn, color = "blue", label = r"\textbf{Vinnikov}")
ax1.legend()

ax2.set(xlabel = r"\textbf{$x$}")
ax2.set(ylabel = r"\textbf{Ratio}")
ax2.set_xlim([0.001, 0.9])
ax2.set_ylim([-0.1, 1.3])
ax2.set_xscale("log")
ax2.plot(xb, fwd/fwd, "k--", lw = 1.5)
ax2.plot(xb, apf/fwd, color = "red")
ax2.plot(xb, vnn/fwd, color = "blue")

newax = f.add_axes([0.8, 0.77, 0.1, 0.1])
newax.imshow(im, alpha = 0.5)
newax.axis('off')

plt.savefig("GPDs_SG_2.5.pdf")
plt.close()

##############################################################
f, ax1 = plt.subplots(1, 1)

ax1.text(0.04, 0.3, r"\textbf{GK model ($\mu_0=2$ GeV)}", fontsize = 18)
ax1.text(0.04, 0.2, r"\textbf{LO evolution}", fontsize = 18)
ax1.text(0.04, 0.13, r"\textbf{$\xi = 0.2$}", fontsize = 18)
ax1.text(0.04, 0.08, r"\textbf{$t=-0.1$ GeV$^2$}", fontsize = 18)
ax1.set(ylabel = r"\textbf{Singlet - (Vinnikov / Forward)}")
ax1.set(xlabel = r"$x$")
ax1.set_xlim([0.001, 0.6])
ax1.set_ylim([0.001, 4])
ax1.set_xscale("log")
ax1.set_yscale("log")
ax1.plot(xb, v2/f2,     label = r"\textbf{$\mu=2$ GeV}",   color = "black", ls = "--", lw = 1.5)
ax1.plot(xb, v2p5/f2p5, label = r"\textbf{$\mu=2.5$ GeV}", color = "blue")
ax1.plot(xb, v3/f3,     label = r"\textbf{$\mu=3$ GeV}",   color = "red")
ax1.plot(xb, v5/f5,     label = r"\textbf{$\mu=5$ GeV}",   color = "green")
ax1.plot(xb, v10/f10,   label = r"\textbf{$\mu=10$ GeV}",  color = "orange")
#plt.plot(xb, apf, color = "red",  label = r"\textbf{APFEL++ (prel.)}")
#plt.plot(xb, vnn, color = "blue", label = r"\textbf{Vinnikov}")
plt.legend()

newax = f.add_axes([0.8, 0.77, 0.1, 0.1])
newax.imshow(im, alpha = 0.5)
newax.axis('off')

plt.savefig("DCVS.pdf")
plt.close()

##############################################################
f, ax1 = plt.subplots(1, 1)
apfnlo10 = make_interp_spline(data[2500:3000,1], data[2500:3000,5], k = 3)(xb)
apfnlo3  = make_interp_spline(data[3000:3500,1], data[3000:3500,5], k = 3)(xb)

ax1.text(0.025, 2.0, r"\textbf{GK model ($\mu_0=2$ GeV)}", fontsize = 18)
ax1.text(0.025, 1.9, r"\textbf{$\xi = 0.2$}", fontsize = 18)
ax1.text(0.025, 1.8, r"\textbf{$t=-0.1$ GeV$^2$}", fontsize = 18)
ax1.set(ylabel = r"\textbf{Singlet forward - (NLO / LO)}")
ax1.set(xlabel = r"$x$")
ax1.set_xlim([0.001, 0.6])
ax1.set_ylim([0.8, 2.5])
ax1.set_xscale("log")
#ax1.set_yscale("log")
ax1.plot(xb, f10/f10,    color = "black", ls = "--", lw = 1.5)
ax1.plot(xb, apfnlo3/f3, label = r"\textbf{$\mu=3$ GeV}",  color = "blue")
ax1.plot(xb, apfnlo10/f10, label = r"\textbf{$\mu=10$ GeV}",  color = "red")
plt.legend(loc = "upper right", bbox_to_anchor=(0.9, 1))

newax = f.add_axes([0.8, 0.77, 0.1, 0.1])
newax.imshow(im, alpha = 0.5)
newax.axis('off')

plt.savefig("NLO_vs_LO.pdf")
plt.close()
