# Intersect

## Design Decisions
I decided to add my implicit equations to new classes instead of my existing classes from shapes. I did this because even though the shapes are similiar, I think the functionality is distinct. These can be found in the `ray` folder. They are all the classes that implement `Intersectable`.

My ray tracing functionality is in the class RayTracer. 

## Extra Credit
Multithreading (see RayTracer::traceScanLine) and supersampling (see RayTracer::traceScanLineSuperSampled). As of right now, you can only super sample when multithreading is enabled. The multithreading works by delegating a thread to each scan line of the image via QTConcurrent::mapped. Each thread is responsible for finding the color values of each pixel in the scan line and then writing it to the canvas. No synchronization structures like mutexes are needed because there are no shared resources.

## Known bugs
None

## Formatting
I'm aware that some of the formatting choices are odd (e.g.: commas to start the line in member initializer lists). I used a clang-format style preset, so these are deliberate (albeit wacky) choices.