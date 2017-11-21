#ifndef SETTING_HPP
#define SETTING_HPP

#include "appsetting.hpp"
#include "capturesetting.hpp"

using namespace std;
using namespace App;

namespace App
{
    class Setting : public CaptureSetting, public AppSetting
    {
    public:
        std::string setPath(std::string path);

    private:
        std::string path;

    };
}  //End of namespace App


#endif // SETTING_HPP
