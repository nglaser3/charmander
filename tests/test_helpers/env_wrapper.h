#ifndef CHARMANDER_TEST_HELPERS_ENV_WRAPPER_H_
#define CHARMANDER_TEST_HELPERS_ENV_WRAPPER_H_

#include <filesystem>
#include <string>

#include "constants.h"
#include "materials/nuclide.h"

#include <gtest/gtest.h>

namespace charmander::test_helpers
{
  class CharmanderXSEnvWrapper {
    protected:
      std::string test_xs_dir_;
      bool has_original_{false};
      std::string old_env_var_;
      std::string nuclide_{"FakeU235"};
      std::string charmander_xs_{"CHARMANDER_CROSS_SECTIONS"};

      void overwrite() {
        const char* var = std::getenv(charmander_xs_.c_str());
        if (var) {
          has_original_ = true;
          old_env_var_ = var;
        }

        // defined in test level cmake, avoids having to determine path at runtime
        std::filesystem::path test_xs_root = CHARMANDER_TEST_DATA_DIR;
        test_xs_dir_ = (test_xs_root / "fake_cross_sections").string();

        #ifdef _WIN32
            _putenv_s(charmander_xs_.c_str(), test_xs_dir_.c_str());
        #else
            setenv(charmander_xs_.c_str(), test_xs_dir_.c_str(), 1);
        #endif
      }

      void reinstate() {
        if (has_original_) {
          #ifdef _WIN32
                _putenv_s(charmander_xs_.c_str(), old_env_var_.c_str());
          #else
                setenv(charmander_xs_.c_str(), old_env_var_.c_str(), 1);
          #endif
              } else {
          #ifdef _WIN32
                _putenv((charmander_xs_ + "=").c_str());
          #else
                unsetenv(charmander_xs_.c_str());
          #endif
        }
      }
  };
}; // namespace charmander::test_helpers


#endif // CHARMANDER_TEST_HELPERS_ENV_WRAPPER_H_