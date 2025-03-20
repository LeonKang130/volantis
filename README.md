# MiniatureAppearance

# Base Material

We want to use voxels to represent miniature (far-away) objects in the scene, making a trade-off between LOD fidelity and rendering performance. We assume that the input objects are represented by triangle meshes and metallic-roughness workflow materials. We parameterize the materials using th following variables:

- **Base Color**: $\beta_x^c$, representing the color of diffuse and/or metallic reflection
- **Roughness**: $\alpha_x^+,\alpha_x^-$, representing the roughness for GGX in specular reflection
- **Metallic-ness**: $\beta^m_x$, a weighting factor for the metallic reflection versus dielectric reflection
- **Index of Refraction**: $\eta_x$, describing the properties of specular dielectric reflection

Using the Schlick-Fresnel approximation, we can write the Fresnel reflectance as:
$$
F(\omega_i,\omega_h;r_0)=r_0(1-F_c)+F_c,F_c=(1-|\omega_i\cdot\omega_h|)^5
$$

For a more convenient parameterization of the base BSDF, we define $\beta^s_x=(1-\eta_x)^2/(1+\eta_x)^2$, representing the specular dielectric reflectance at normal incidence.

Using the parameterization above, we can write the base BSDF as:

$$
\begin{align}
f_\mathrm{disney}(\omega_i,\omega_o)&=f_d(\omega_i,\omega_o)+f_s(\omega_i,\omega_o)\\
f_d(\omega_i,\omega_o)&=\frac{(1-\beta_x^m)\beta^c_x}{\pi}\\
f_s(\omega_i,\omega_o)&=\frac{D(\omega_h;\alpha_x)G(\omega_i,\omega_o,\alpha_x)}{4|n_x\cdot\omega_i||n_x\cdot\omega_o|}\left((1-\beta_x^m)F(\omega_i,\omega_h,\beta^s_x)+\beta^m_xF(\omega_i,\omega_h,\beta^c_x)\right)\\
\end{align}
$$

## Far-Field Approximation and ABSDF

The rendering equation describing the light transport at a point $x$ on a subset of some surfaces in the scene $A$ is given by:

$$
L_o(x,\omega_o)=\int_{S^2}L_i(x,\omega_i)f(x,\omega_i,\omega_o)\langle n_x,\omega_i\rangle\mathrm{d}\omega_i
$$

When viewed from far away, we consider the $L_i$ to be constant within the neighborhood of $x$, giving the following approximation:

$$
L_o(x,\omega_o)=\int_{S^2}L_i(\omega_i)f(x,\omega_i,\omega_o)\langle n_x,\omega_i\rangle\mathrm{d}\omega_i
$$

Averaging the outgoing radiance over $A$, we have

$$
L_o(\omega_o)=\frac1{|A|}\int_{S^2}L_i(\omega_i)\int_Af(x,\omega_i,\omega_o)V(x,\omega_i)V(x,\omega_o)\langle n_x,\omega_i\rangle\mathrm{d}x\mathrm{d}\omega_i
$$

We included the visibility terms $V(x,\omega_i)$ and $V(x,\omega_o)$ to account for self-occlusion within the surface subset $A$. The ABSDF is thus defined as:

$$
\hat f(\omega_i,\omega_o)=\frac1{|A|}\int_Af(x,\omega_i,\omega_o)V(x,\omega_i)V(x,\omega_o)\langle n_x,\omega_i\rangle\mathrm{d}x
$$

Assume that the visibility terms are independent of the BSDF, we can factorize the ABSDF as two terms representing aggregated appearance and visibility respectively:

$$
\hat f(\omega_i,\omega_o)\approx\frac1{|A|}\int_Af(x,\omega_i,\omega_o)\langle n_x,\omega_i\rangle\mathrm{d}x\cdot\frac1{|A|}\int_AV(x,\omega_i)V(x,\omega_o)\mathrm{d}x
$$

In this project we focus on the aggregated appearance term.

## Problem Description

In our setting we want to fit the approximated aggregated appearance fast, which prevents us from sweeping over the entire domain of integration $A$. What we can do is to scatter a few samples within $A$ and use them to capture information about the underlying base BSDF and distribution of surface normal. If the scene stays static, we will be able to obtain a good LOD result after accumulating enough samples.
