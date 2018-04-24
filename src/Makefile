
main:camera.o shader.o glad.o imgui.o imgui_demo.o imgui_draw.o imgui_impl_glfw_gl3.o
	g++ -o main main.cpp camera.o shader.o glad.o imgui.o imgui_demo.o imgui_draw.o imgui_impl_glfw_gl3.o -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
camera.o:
	g++ -c Camera.cpp
shader.o:
	g++ -c Shader.cpp
glad.o:
	g++ -c glad/glad.c
imgui.o:
	g++ -c ./imGui/imgui.cpp ./imGui/imgui_impl_glfw_gl3.cpp ./imGui/imgui_draw.cpp ./imGui/imgui_demo.cpp
clean :
	rm -f *.o