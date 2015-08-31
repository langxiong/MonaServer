#include "MyServer.h"
#include "Mona/ServerApplication.h"

using namespace std;
using namespace Mona;

class ServerXL : public ServerApplication  {

private:

    void defineOptions(Exception& ex, Options& options) {
        options.acceptUnknownOption = true;
        ServerApplication::defineOptions(ex, options);
    }

    ///// MAIN
    int main(TerminateSignal& terminateSignal) {

        std::string applicationDir;
        this->getString("application.dir", applicationDir);
        this->setString("blockListFilePath", applicationDir + "blockList");
        // starts the server
        MyServer server(*this, terminateSignal);
        if (server.start(*this)) {
            terminateSignal.wait();
            // Stop the server
            server.stop();
        }
        return Application::EXIT_OK;
    }

};

int main(int argc, const char* argv []) {
    return ServerXL().run(argc, argv);
}
