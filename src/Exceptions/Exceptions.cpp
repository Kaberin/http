#include "Exceptions.hpp"

namespace web {
    namespace exceptions {
        void HTTPException::Print() {
            Logger::GetInstance().Log(LogType::ERROR, what());
        }
    }  // namespace exceptions

}  // namespace web
