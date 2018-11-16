# 2D-Net
2D net construction using molecular dynamics approach

The goal is to construct the net on the 2D surface. 

The main idea is to imagine, that the nodes of the net is particles with similar charge.
Put those particles on the surface randomly. 

That means that according to the Coulomb's Law they will fall apart. 
If we will put the particles, with bigger charges on the border of the surface, the small particles inside it would oscillate, 
and won't fly away from our surface. If after that we will add viscosity, they will take approximately optimal places and 
won't be oscillating any more. 

This is how it happens:


Orange dots - border. Dots inside randomly placed in corners. 
![alt text](https://github.com/EkaterinaO/2D-Net/blob/master/step1.png)
After few iterations. 
![alt text](https://github.com/EkaterinaO/2D-Net/blob/master/step2.png)
The final net. Now we can perform Delaunay triangulation, for example. 
![alt text](https://github.com/EkaterinaO/2D-Net/blob/master/step3.png)

