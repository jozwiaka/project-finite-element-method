import numpy as np
from scipy import special


def Bi(alpha, r0, lambda1):
    return alpha * r0 / lambda1


def Fo(r0, lambda1, t, c, rho):
    return lambda1 * t / (c * rho * r0**2)


def ss_bc1(x, qv, lambdax, L, T1, T2):
    return qv * L**2 / (2 * lambdax) * (x/L - (x/L)**2) - (T1 - T2)/L * x + T1


def ts_bc1(x, t, T0, lambdax, rho, c, T1):
    return T1 + (T0 - T1) * special.erf(x / (2 * np.sqrt(lambdax * t / (rho * c))))


def ts_bc2(x, t, T0, lambda1, rho, c, qs):
    return T0 + qs/lambda1 * (np.sqrt(4 * lambda1/(rho * c) * t/np.pi) * np.exp(-x**2 / (4 * lambda1 / (rho * c) * t)) - x * special.erfc(x / (2 * np.sqrt(lambda1/(rho * c) * t))))


def ts_bc3(x, t, T0, lambda1, rho, c, T1, alpha):
    return T0 + (T1 - T0) * (special.erfc(x / (2 * np.sqrt(lambda1/(rho * c) * t))) - np.exp(alpha * x / lambda1 + alpha**2 * lambda1/(rho * c) * t / (lambda1**2)) * special.erfc(x / (2 * np.sqrt(lambda1 / (rho * c) * t)) + alpha * np.sqrt(lambda1/(rho * c) * t)/lambda1))


def Txt_konw1(r, t, T1, T0, lambda1, rho, c, alpha, r0, A1, mu1):
    return T1 + (T0 - T1) * A1 * np.cos(mu1 * r / r0) * np.exp(-mu1**2 * Fo(r0, lambda1, t, c, rho))


def Trt_konw1(r, t, T1, T0, lambda1, rho, c, alpha, r0, A1, mu1):
    return T1 + (T0 - T1) * A1 * special.j0(mu1 * r / r0) * np.exp(-mu1**2 * Fo(r0, lambda1, t, c, rho))


# formulas:
# f_ss_bc1 = r'$\dfrac{\dot{q_v} a^2}{2 \lambda}\left(\dfrac{x}{a} - \dfrac{x^2}{a^2}  \right) - \dfrac{T_{1} - T_{2}}{a}x + T_1$'
# f_ts_bc1 = r'$T_s+(T_0-T_s)\text{erf}\left(\dfrac{\sqrt{\rho c_w}x}{2\sqrt{\lambda t}}\right)$'
# f_ts_bc2 = r'$T_0 + \dfrac{\dot{q_s}}{\lambda}\left[\sqrt{\dfrac{4 \lambda t}{\pi \rho c_w}}\text{exp}\left(-\dfrac{x^2\rho c_w}{4 \lambda t}\right) - x\,\text{erfc}\left(\dfrac{x\sqrt{\rho c_w}}{2\sqrt{\lambda t}}\right) \right]$'
# f_ts_bc3 = r'$T_0 + (T_{\infty}-T_0)\left(\text{erfc}\left(\dfrac{x\sqrt{\rho c_w}}{2\sqrt{\lambda t}} \right) - \text{exp}\left(\dfrac{hx}{\lambda} + \dfrac{h^2 t}{\lambda\rho c_w}\right)\text{erfc}\left(\dfrac{x \sqrt{\rho c_w}}{2\sqrt{\lambda t}} + \dfrac{h\sqrt{\lambda t}}{\lambda\sqrt{\rho c_w}} \right)\right)$'

f_ss_bc1 = r'$\dfrac{\dot{q_v} c^2}{2 \lambda}\left(\dfrac{z}{c} - \dfrac{z^2}{c^2}  \right) - \dfrac{T_{1} - T_{2}}{c}z + T_1$'
f_ts_bc1 = r'$T_s+(T_0-T_s)\text{erf}\left(\dfrac{\sqrt{\rho c_w}z}{2\sqrt{\lambda t}}\right)$'
f_ts_bc2 = r'$T_0 + \dfrac{\dot{q_s}}{\lambda}\left[\sqrt{\dfrac{4 \lambda t}{\pi \rho c_w}}\text{exp}\left(-\dfrac{z^2\rho c_w}{4 \lambda t}\right) - z\,\text{erfc}\left(\dfrac{z\sqrt{\rho c_w}}{2\sqrt{\lambda t}}\right) \right]$'
f_ts_bc3 = r'$T_0 + (T_{\infty}-T_0)\left(\text{erfc}\left(\dfrac{z\sqrt{\rho c_w}}{2\sqrt{\lambda t}} \right) - \text{exp}\left(\dfrac{hx}{\lambda} + \dfrac{h^2 t}{\lambda\rho c_w}\right)\text{erfc}\left(\dfrac{z \sqrt{\rho c_w}}{2\sqrt{\lambda t}} + \dfrac{h\sqrt{\lambda t}}{\lambda\sqrt{\rho c_w}} \right)\right)$'

f_Txt_konw1 = r'$T_{\infty} + (T_0 - T_{\infty})A_1\text{exp}\left(-\lambda^2_1\tau\right)\text{cos}\left(\lambda_1\dfrac{x}{L}\right)$'
f_Trt_konw1 = r'$T_{\infty} + (T_0 - T_{\infty})A_1\text{exp}\left(-\lambda^2_1\tau\right)J_0\left(\lambda_1\dfrac{r}{r_0}\right)$'
#formula_latex = {'Txt_T1': wzorTxt_T1, 'Txt_str': wzorTxt_str, 'Txt_konw': wzorTxt_konw, 'Txt_konw1': wzorTxt_konw1, 'Trt_konw1': wzorTrt_konw1}
