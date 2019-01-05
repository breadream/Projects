#ifndef PROJECT_INCLUDE_READ_INPUT_H_
#define PROJECT_INCLUDE_READ_INPUT_H_

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
namespace fcal {
namespace scanner {

/*******************************************************************************
 * Functions
 ******************************************************************************/
char *ReadInput(int argc, char **argv);
char *ReadInputFromFile(const char *filename);

} /* namespace scanner */
} /* namespace fcal */
#endif  // PROJECT_INCLUDE_READ_INPUT_H_
