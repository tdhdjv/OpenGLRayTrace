# A SIMPLE RAYTRACER MADE WITH OPENGL

This is a (slow, and very bad) ray tracer made in c++ using openGL. Currently This program contains spheres, BVH (Bounding Volume Hierarchy), and model loading. It was made with [Ray Tracing in one weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html), and [Sebastian Lague's Ray Tracing](https://github.com/SebLague/Ray-Tracing) as a reference.

![스크린샷 2025-04-09 160913](https://github.com/user-attachments/assets/d15a6a5c-8adb-40fd-9d76-7343a8ab8839)

![스크린샷 2025-05-09 212120](https://github.com/user-attachments/assets/5d240b0d-e51a-4bc3-884b-f880c0261f52)

# Features of the Ray Tracer
Here are the list of features that are in this ray tracer
- Sphere Ray Tracing
- Triangle Ray Tracing
- BVH (Bounding Volume Hiearchy)
- Camera movement
- Model Loading
- Model Transforms (Position, Rotation, Scale)

# Extra Notes
I still consider myself a beginner when it comes to c++, openGL, and programming in general so yes the code is really bad.<br/>
Yes the Codes is really shit and very crytic, I hope that I can improve upon that in the next opengl Project.

# Unhinged Rant about Overengineering, OOP, and my STUPIDITY
This section of the ReadMe has nothing to do with the ray tracer but, instead about the development process of the ray tracer and how Over engineering, OOP kinda ruined everything.<br/><br/>
**Note**: that this is coming from my experience making this ray tracer. You can have your own opion and frankly it's probably going to be better than mine (I'm a beginner after all). If you have any thing you disagree with give me some feedback and also please be Nice! :) thank you!<br/>

## Why is this here?
Still I wanted to have a talk about because I'm really frustrated and need and outlet to talk about my feeling.

## Part 1. The beginning
When I started developing the Ray Tracer, I had simple goals.<br/>
1. It had to ray trace (obviously)
1. Was able to load any model/create a BVH data.
1. The Shader was able to use the BVH data to only check a limited about of triangles for every ray<br/>
that was it.<br/>
This was going to be a simple beginner friendly project to have fun, and familiarize myself with OpenGL, and the concept of rayTracing.<br/><br/>

My first implementation was to basically have every feature in the "main.cpp" file and in the ```int main()``` function. Obviously I made classes for stuff like shaders, models(loading models from .obj files), vertex buffers(VBO), index buffers(EBO) and vertex arrays(VAO)(note. this will be relevant later). But generally, the logic of the program was mostly only in the main function. At this point everything worked!. I've implemented the ablity to load models, create BVH data, and pass that data to the shader to use it in the BVH. Everything worked fine and perfectly, The Ray tracing was slow, but I already expected that, and that wasn't the primary goal of the program. The code might have not been the prettiest out there, but everything work fine, I understood how the program flowed. I was happy :) and every was fine.<br/><br/>

## Part 2. Unnecassary Abstraction
Now after this I thought there is 2 paths I can go down. <br/>
1. I do a little bit of research and optimize my very slow Ray tracer,
2. refactor the code and abstract the code to work more generally.
Now as it was stated before, I have already achieved the intial goals of the program, and I didn't really need to have multiple scenes or have the ablity to use any arbitary shaders. So from this fact It was obvious... I chose option 2... like a fucking idiot.<br/><br/>

I decided that I was going to design a system in which you can render objects using any shader you want, and have different scenes, which you can switch between. so I started to abstract. I made a ```Scene``` class, ```Renderer``` class, so on and so forth. and I started to make abstractions left and right dividing up feature, making them more abstract and general, I made a really stupid abstract called A "RenderPass" where it would describe "the smallest unit of collective render" whatever that meant, which made rendering a pain in the ass to deal with. However, I didn't know this at the time and natively thought "Everything is great", but it wasn't all great. The system was over engineered for what the original goal of the program was, and I had loads of different files, that I had to navigate through inorder to see what was going on, and to debug my code. So I sacrificed a bunch of time, effort and code readablity, in order to achieve this vague and unnessary notion of abstractness, and making the program more general.

## Part 3. OOP RUINED MY LIFE!!!
Remember how I said that I had class for vertex arrays? well, since in openGL after you are done using buffers, you need to delete them inorder to avoid memory leaks. after I saw this my mind immediately went to RAII(Resource Acqusition is Initialization). Everything was fine. That was until, I had to pass around the vertex buffers, and index buffers. <br/><br/>

The current system worked like this. First I would make a "Mesh" in the Application, then we would pass this mesh in to a Scene where it will be stored in a vector of Meshes. The mesh class had a VertexArray(which like mentioned previously used RAII) as a member variable. A problem arose when passing around this Mesh class. If I made a copy of the objects. There will be 2 VertexArray objects with the same id. When the destructor of this objects was called, the they would delete the array object with that id. However when the destructor was called twice, OpenGL would try to delete the array twice buffer which isn't allowed. <br/>
"Well, Shit!" I thought to myself. This was probably a sign of bad things to come. Now, there was 2 ways to fix this, 1. use smart pointers such that there is only one instance of the object in memory. or 2 not allow copies, and only use move semantics to pass around data.<br/><br/> I didn't want to use smart pointers because I thought that smart pointers would just lead to memory fragmentation, and also was generally unnessary. (This choices would become one of the biggest headaches for me). As I started using move sematics I still was getting the same double deletion error. I thought that this was strange. Apparently moving the data doesn't mean, that the destrutor isn't called and therefore the issue wasn't fixed. This should have been the sign that I was making the wrong choice. GO BACK!!! the universe was telling me. however I ignored those warning and instead made a crappy fix to this issue and the code worked. However I kept getting errors, I kept getting cryptic error messages about "construct_at not found" and, it was real pain in the ass to find, and fix all of these.<br/><br/>

This wasn't limited to just VertexArrays I had to deal with the same problem in ShaderProgram, and ShaderStorageBuffer.  <br/>
Note: The Problem may have not been OOP but, mostly RAII since, OpenGL objects should probably be deleted explictly to avoid this type of headache. I'm not sure about this and would love some feedback on this.<br/><br/>

OOP also made the code needlessly complicated, I made the render function be a member of a Renderer class which is a common practice from what i can see. but was unnessary for this project. I also had to make clases like the RayTracingManager, and all this abstract manager, and do-ers. I didn't really like this, it felt like these where unnessary and where only function holder(class for the sake of class and not really a dataType). OOP also made it harder, to navigate through my code base, yes this is mostly due to the overengineering, but I think OOP also played a part in this.

## Part 4. Conclusion
So, what was the point of all this. First I wanted to reflect back on the decision that I made, and how I can improve from my past mistakes, Secondly, I need a hug (not literally, don't touch me).<br/><br/>

To Summarize:
I had code that was simple and was pretty much just procedural code, then I tried to abstract my code, and make it overly engineered, and introducted OOP Ideas and it lead to disater.<br/><br/>

I think that have some I can learn some lessons from this experience.
1. Don't Fix A problem That doesn't exist.
2. Use OOP resposible(I don't think OOP is the worst thing ever, but it can lead to alot of headaches).
3. If something isn't right Stop and think back at if you are doing the right thing. (think before you code)
<br/>

Thank You for anyone who took the time to read all this shit, I hope I can recieve some helpful feedback on How to Structure my code, what I can improve on, and general Programming tips!<br/>
Thank you and GoodBye.
