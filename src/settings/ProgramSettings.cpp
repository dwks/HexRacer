#include "ProgramSettings.h"

namespace Project {
namespace Settings {

ProgramSettings *ProgramSettings::instance = 0;

ProgramSettings::ProgramSettings(bool client) {
    this->client = client;
    connected = false;
    instance = this;
}

}  // namespace Settings
}  // namespace Project
