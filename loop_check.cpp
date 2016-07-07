#include <iostream>
#include <iomanip>
#include <unordered_set>
#include "Array2D.hpp"

#define NO_FLOW          0 //TODO: Explain and ensure it fits in flowdir_t

template<class flowdir_t>
void FollowPath(
  const int x0,                      //x-coordinate of initial cell
  const int y0,                      //y-coordinate of initial cell
  const Array2D<flowdir_t> &flowdirs //Flow directions matrix
){
  int x = x0;
  int y = y0;

  size_t path_len        = 0;
  size_t max_path_length = flowdirs.viewWidth()*flowdirs.viewHeight(); //TODO: Should this have +1?
  max_path_length        = flowdirs.viewWidth();

  //Follow the flow path until it terminates
  while(path_len++<max_path_length){ //Follow the flow path until we reach its end
    const int n = flowdirs(x,y);     //Neighbour the current cell flows towards

    //Show the final part of the loop path (TODO)
    if(path_len>max_path_length-10)
      std::cout<<x<<","<<y<<" with flowdir "<<n<<"\n";

    if(flowdirs.isNoData(x,y) || n==NO_FLOW)
      return;

    const int nx = x+dx[n]; //Get neighbour's x-coordinate.
    const int ny = y+dy[n]; //Get neighbour's y-coordinate.

    if(!flowdirs.in_grid(nx,ny))
      return;

    x = nx;
    y = ny;
  }

  std::cout<<"\n\n";
}



template<class T>
void Master(std::string input){
  Array2D<T> inp(input, 0, 0, 0, 0, false);

  for(size_t y=0;y<inp.viewHeight();y++)
  for(size_t x=0;x<inp.viewWidth();x++)
    FollowPath(x,y,inp);
}

int main(int argc, char **argv){
  int32_t total_height;
  int32_t total_width;
  GDALDataType file_type;

  std::string inputfile;

  if(argc!=2){
    std::cerr<<argv[0]<<" <FILE>"<<std::endl;
    return -1;
  }

  inputfile = argv[1];

  //Get the total dimensions of the input file
  if(getGDALDimensions(inputfile, total_height, total_width, file_type, NULL)!=0){
    std::cerr<<"Error getting file information from '"<<inputfile<<"'!"<<std::endl;
    return -1;
  }

  switch(file_type){
    case GDT_Byte:
      Master<uint8_t >(inputfile);break;
    case GDT_UInt16:
      Master<uint16_t>(inputfile);break;
    case GDT_Int16:
      Master<int16_t >(inputfile);break;
    case GDT_UInt32:
      Master<uint32_t>(inputfile);break;
    case GDT_Int32:
      Master<int32_t >(inputfile);break;
    case GDT_Float32:
      Master<float   >(inputfile);break;
    case GDT_Float64:
      Master<double  >(inputfile);break;
    default:
      std::cerr<<"Unrecognised data type!"<<std::endl;
      return -1;
  }
}