#import <Foundation/Foundation.h>
#include <imgui_utils.hpp>

std::string ImGuiUtils::GetCurrentPath()
{
    @autoreleasepool {
        NSString *projectDir = PROJECT_DIR_PATH;
        // UTF-8 文字列に変換して std::string に
        return std::string([projectDir fileSystemRepresentation]);
    }
}
