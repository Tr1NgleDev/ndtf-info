#include <iostream>
#include <ndtf/ndtf.h>
#include <filesystem>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("Usage: %s <ndtf file>\n", argv[0]);
		return 1;
	}

	if (!std::filesystem::exists(argv[1]))
	{
		printf("Unable to load \"%s\" (file doesn't exist)\n", argv[1]);
		return 2;
	}

	NDTF_File file = ndtf_file_load(argv[1], nullptr, NDTF_TEXELFORMAT_NONE);
	if (!ndtf_file_isValid(&file))
	{
		printf("Unable to load \"%s\" (invalid/unsupported NDTF)\n", argv[1]);
		return 3;
	}

	printf("File Path: %s\n", argv[1]);
	printf("NDTF v%i.%i\n", NDTF_EXTRACT_VERSION_MAJOR(file.header.version), NDTF_EXTRACT_VERSION_MINOR(file.header.version));
	uint64_t totalTexels = 1;
	for (int i = 0; i < file.header.dimensions; ++i)
	{
		totalTexels *= file.header.size[i];
		if (i == file.header.dimensions - 1)
			printf("%i", file.header.size[i]);
		else
			printf("%i x ", file.header.size[i]);
	}
	printf(" (%iD; %llu texels)\n", file.header.dimensions, totalTexels);

	const char* channelsStr = "Gray-scale (R)";
	if (ndtf_getChannelCount((NDTF_TexelFormat)file.header.texelFormat) == 3)
		channelsStr = "RGB";
	else if (ndtf_getChannelCount((NDTF_TexelFormat)file.header.texelFormat) == 4)
		channelsStr = "RGBA";

	printf("%i-bit %s %i channels (%s)\n",
		(int)ndtf_getChannelSize((NDTF_TexelFormat)file.header.texelFormat) * 8,
		(ndtf_getChannelIsFloat((NDTF_TexelFormat)file.header.texelFormat) ? "float" : "int"),
		(int)ndtf_getChannelCount((NDTF_TexelFormat)file.header.texelFormat),
		channelsStr);

	printf(
		"Flags:\n"
		"\tZ-Lib Compression: %s\n"
		,file.header.flags.zlib_compression ? "Yes" : "No"
	);

	ndtf_file_free(&file);

	return 0;
}
