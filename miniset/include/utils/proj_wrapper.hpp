#ifndef MINISET_UTILS_PROJ_WRAPPER_HPP
#define MINISET_UTILS_PROJ_WRAPPER_HPP

#include <string>

namespace utils {

#ifdef MINISET_HAS_PROJ
// PROJ transformer wrapper
class ProjTransformer {
public:
    ProjTransformer(const std::string& source_proj, const std::string& dest_proj);
    ~ProjTransformer();
    
    void transform(double& x, double& y, double& z);
    
private:
    void* ctx_;  // PJ_CONTEXT*
    void* proj_; // PJ*
};
#endif

} // namespace utils

#endif
