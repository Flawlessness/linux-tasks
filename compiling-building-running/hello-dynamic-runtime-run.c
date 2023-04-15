#include <stdio.h>
#include <dlfcn.h>

int main (int argc, char* argv[])
{	
	void *ext_library;
	void (*hello_func)();

	ext_library = dlopen("./libhello-dynamic-runtime.so", RTLD_LAZY);
	if (!ext_library){
		fprintf(stderr, "dlopen() error: %s\n", dlerror());
		return 1;
	};

	hello_func = dlsym(ext_library, "hello_from_dyn_runtime_lib");

	(*hello_func)();

	dlclose(ext_library);

	return 0;
}
