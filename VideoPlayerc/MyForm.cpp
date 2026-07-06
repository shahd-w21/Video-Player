#include "MyForm.h"
#include "MenuForm.h"


using namespace VideoPlayerc;

[STAThreadAttribute]
int main(array<System::String^>^)
{
    // Entry point for the application: initializes visual styles and
    // opens the MenuForm so the user can choose Video or Photo player.
    Application::EnableVisualStyles();
    // Start application with the MenuForm so user chooses Video or Photo player first
    Application::Run(gcnew MenuForm());
    return 0;
}
