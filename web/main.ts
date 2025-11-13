// A simple base64 encoded 1x1 black pixel PNG.
// In a real scenario, this would be replaced with a sample frame from the Android app.
const sampleFrameBase64 = 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAQAAAC1HAwCAAAAC0lEQVR42mNkYAAAAAYAAjCB0C8AAAAASUVORK5CYII=';

// A more complex placeholder: a 10x10 checkerboard pattern
const checkerboardBase64 = 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAoAAAAKCAYAAACNMs+9AAAAHElEQVQoU2NkYGD4z/AdjAxAccepted/+//9/AAnQZ1s+y9AhAAAAAElFTkSuQmCC';


document.addEventListener('DOMContentLoaded', () => {
    const frameImage = document.getElementById('processed-frame') as HTMLImageElement;
    const resolutionSpan = document.getElementById('resolution');
    const fpsSpan = document.getElementById('fps');

    if (frameImage && resolutionSpan && fpsSpan) {
        // Use the checkerboard as a more visible placeholder
        frameImage.src = checkerboardBase64;

        // Simulate loading image properties
        const img = new Image();
        img.onload = () => {
            resolutionSpan.textContent = `${img.width}x${img.height}`;
        };
        img.src = checkerboardBase64;

        fpsSpan.textContent = 'N/A (Static Image)';
    }
});
