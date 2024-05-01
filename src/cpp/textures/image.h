#ifndef IMAGE_H_
#define IMAGE_H_


struct Image {

    int width = 0;
    int height = 0;
    int components = 0;
    float *data = nullptr;
    // Constructor
    Image(const std::string& filename);
    // Destructor
    ~Image();;

};


#endif //IMAGE_H_
