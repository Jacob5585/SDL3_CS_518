#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::string sanitize_identifier(const std::string &filepath) {
    std::string name = filepath;

    size_t last_slash = name.find_last_of("/\\");
    if (last_slash != std::string::npos) {
        name = name.substr(last_slash + 1);
    }
    // Replace non-alphanumeric characters with underscores
    for (char &c : name) {
        if (!isalnum(c)) {
            c = '_';
        }
    }
    return name;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "No argumetns passed\n";
        return 1;
    }

    // header file
    std::ofstream header("assets.h");
    header << "#ifndef __STUFF_H__" << std::endl;
    header << "#define __STUFF_H__" << std::endl;
    header << "#include <SDL3/SDL.h>" << std::endl << std::endl;

    for (int i = 1; i < argc; i++) {
        std::string ident = sanitize_identifier(argv[i]);
        header << "extern const unsigned int " << ident << "_len;" << std::endl;
        header << "extern const Uint8 " << ident << "[];" << std::endl;
    }

    header << std::endl << "#endif /* __ASSETS_H__ */" << std::endl;
    header.close();

    // source file
    std::ofstream source("assets.cpp");
    source << "#include \"assets.h\"" << std::endl << std::endl;

    for (int i = 1; i < argc; i++) {
        std::string filepath = argv[i];
        std::string ident = sanitize_identifier(argv[i]);

        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            std::cerr << "Could not open file: " << filepath << std::endl;
            continue;
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        if (!file.read(buffer.data(), size)) {
            std::cerr << "Could not read file: " << filepath << std::endl;
            continue;
        }

        // Write length
        source << "const unsigned int " << ident << "_len = " << size << ";" << std::endl;
        // Write byte array
        source << "const Uint8 " << ident << "[] = {" << std::endl;

        for (std::streamsize j = 0; j < size; j++) {
            source << "0x" << std::hex << (int)(unsigned char)buffer[j] << std::dec << ", ";
            if ((j + 1) % 12 == 0) source << std::endl;
        }

        source << std::endl << "};" << std::endl << std::endl;
    }
    source.close();
    std::cout << "Successfully generated assets.h and assets.cpp!" << std::endl;
    return 0;
}