//#include

int main()
{
    // Read images / video

    // Create image calibrator from the chessboard images
        // defensive: check dimension of all chessboard images

    // for every image/frame

        // Calibrate the image with the calibrator above
            // defensive: check dimension image

        // Apply binary thresholding on the image

        // Rectify image to BEV

        // Detect lane pixels and fit to the lane boundary

        // Calculate curvature and vehicle offset from the lane center

        // Warp the detected lane boundaries back to original image

        // Visualize lane, boundaries, curvature, center offset

        // Display the image

    return 0;
}
