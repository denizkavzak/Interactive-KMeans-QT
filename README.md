# Interactive-KMeans-QT

This repository consists of an interactive K-Means Clustering implemented in C++ with QT. 
The K-means implementation can work for any dimensions greater than or equal to 1.
Visualization of the clustering process is done for 2D and 3D cases. 

# Running the code:

This implementation is done using QTCreator. 
Project can be opened and run using QTCreator and provided K-Means-Project.pro file. 

Project has following dependencies in QT:
QT Charts and 
QT Data Visualization. 

# Implemented tool allows user to:
1. Generate/Import data vectors with any dimension
2. Change distance metric in between Euclidean/Manhattan/Hamming for binary features
3. Initialize cluster centers using Random Sample/Random Real/K-Means++
4. Cluster points step by step/via animation until algorithm is converged/maximum number of iterations is reached


# User interactions:
1. In data generation:
- Enter minimum and maximum boundaries for data generation
- Enter number of points
- Enter desired dimension of data to be clustered


2. In clustering initialization:
- Enter k (number of clusters)
- Enter the maximum number of iteration
- Select distance metric
- Select initialization method
- Manually select cluster centers (Supported only in 3D)
- Rotate (Supported only in 3D)

3. In clustering:
- Iterate step by step in clustering
- Run clustering for the maximum number of iterations and get final result
- Go back one step in clustering
- Animate step by step in clustering by entering the desired animation rate until the algorithm converges (Stop and continue anytime)

4. Interactions with the graphs for both 2D and 3D:
- Zoom in/Zoom out/ Panning 
- Change point size in the graph
- Reset the clustering to start with a new dataset

