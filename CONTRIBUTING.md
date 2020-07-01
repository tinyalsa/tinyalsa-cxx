Here's some general rules of thumb when contributing to this project.
If there's a question about any of these, feel free to create an issue with the question.

 - All code should be properly indented and consistent with the rest of the C++ style.
 - Avoid using exceptions and note them in documentation if they must be used.
 - Pull requests should not be larger than 200 lines of code.
 - Good pull requests are minimal change sets (don't add multiple features in one PR.)
 - This project is not affiliated with Android.
   PRs that support Android are welcome but whether or not they're accepted is based on what's best for this project.
 - Keep what's exposed in the header file to a bare minimum.
 - Any functions that are added to the header file should be formally documented with Doxygen.
 - When possible, write a test (automated if possible) that verifies the behavior of a function.
