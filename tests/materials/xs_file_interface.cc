#include <exception>
#include <cstdlib>
#include <filesystem>

#include <gtest/gtest.h>
#include <hdf5.h>

#include "materials/xs_file_interface.h"

namespace charmander
{
  // Environment variable guard to check the env var getter actually throws properly
  // Windows makes this complicated. Like always. 
  class TemporaryDeleteEnvVar
  {
    public:
      TemporaryDeleteEnvVar(const std::string& variable) : variable_(variable) {
        const char* var = std::getenv(variable_.c_str());
        if (var)
        {
          has_original_= true;
          original_ = var;
        }
          
        #ifdef _WIN32
          _putenv((variable_ + "=").c_str());
        #else
          unsetenv(variable_.c_str());
        #endif
      };

      ~TemporaryDeleteEnvVar() {
        if (has_original_) {
          #ifdef _WIN32
            _putenv_s(variable_.c_str(), original_.c_str());
          #else
            setenv(variable_.c_str(), original_.c_str(), 1);
          #endif
        }
      }
    protected:
      const std::string variable_;
      bool has_original_{false};
      std::string original_;
  };

  TEST(XSFileInterfaceEnvVar, EnvVarNotFound) {
    TemporaryDeleteEnvVar VARIABLE_GUARD("CHARMANDER_CROSS_SECTIONS");
    EXPECT_THROW(XSFileInterface::ResolveFilePath("U235"), std::runtime_error);
  }

  // fixture so we dont have to write the path grab and init a bunch
  // also want all tests to fail if the env var is not found.
  class MaterialsXSFileInterface: public ::testing::Test
  {
    protected:
      std::string xs_path_;
      std::string nuclide_;
      std::unique_ptr<XSFileInterface> interface_;
      std::string test_xs_dir_;
      bool has_original_{false};
      std::string old_env_var_;
      std::string charmander_xs_{"CHARMANDER_CROSS_SECTIONS"};

      void SetUp() override {

        // tell hdf5 to be quiet, it has ugly error output that pollutes test output
        H5Eset_auto(H5E_DEFAULT, nullptr, nullptr);

        const char* var = std::getenv(charmander_xs_.c_str());
        if (var)
        {
          has_original_= true;
          old_env_var_ = var;
        }
        
        // defined in test level cmake, avoids having to determine path at runtim
        std::filesystem::path test_xs_root = CHARMANDER_TEST_DATA_DIR;
        test_xs_dir_ = (test_xs_root / "fake_cross_sections").string();

        #ifdef _WIN32
          _putenv_s(charmander_xs_.c_str(), test_xs_dir_.c_str());
        #else
          setenv(charmander_xs_.c_str(), test_xs_dir_.c_str(), 1);
        #endif

        const char* gotten = std::getenv(charmander_xs_.c_str());
        ASSERT_EQ(std::string(gotten), test_xs_dir_);
        xs_path_ = gotten;

        nuclide_ = "FakeU235";
        ASSERT_NO_THROW(interface_ = std::make_unique<XSFileInterface>(nuclide_));
      }

      void TearDown() override {
        if (has_original_)
        {
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

  TEST_F(MaterialsXSFileInterface, ConstructorBadNuclide) {
    EXPECT_THROW(XSFileInterface("not_real"), std::runtime_error);
  }

  TEST_F(MaterialsXSFileInterface, ResolveFilePath) {
    std::filesystem::path expected_root = xs_path_;
    std::filesystem::path expected = expected_root / "FakeU235.h5";
    EXPECT_EQ(expected, interface_->ResolveFilePath("FakeU235"));
    EXPECT_THROW(interface_->ResolveFilePath("u235"), std::runtime_error);
  }

  TEST_F(MaterialsXSFileInterface, OpenXSFile) {
    std::string filepath = interface_->ResolveFilePath(nuclide_);
    hid_t file_id = -1;

    EXPECT_NO_THROW(file_id = interface_->OpenXSFile(filepath));
    EXPECT_GE(file_id, 0);

    EXPECT_THROW(interface_->OpenXSFile("not_real.h5"), std::runtime_error);
  }

  TEST_F(MaterialsXSFileInterface, CloseXSFile) {
    // first close call, should not throw
    EXPECT_NO_THROW(interface_->CloseXSFile());
    // second close call, should do nothing and not throw
    EXPECT_NO_THROW(interface_->CloseXSFile());
  }

  TEST_F(MaterialsXSFileInterface, LoadEvaluationEnergies) {
    std::vector<double> energies;

    ASSERT_NO_THROW(interface_->LoadEvaluationEnergies("294K", energies));
    EXPECT_FALSE(energies.empty());
    EXPECT_GE(energies.front(), 0.0);
    for (size_t i = 0; i < energies.size() - 1; i++)
    {
      ASSERT_GE(energies.at(i+1), energies.at(i));
    }

    // invalid dataset path
    EXPECT_THROW(interface_->LoadEvaluationEnergies("293K", energies), std::runtime_error);
    // invalid formatted dataset (char instead of float in this case)
    XSFileInterface bad_file("BadFakeU235");
    EXPECT_THROW(bad_file.LoadEvaluationEnergies("294K", energies), std::runtime_error);
  }

  TEST_F(MaterialsXSFileInterface, GetEnergyPath) {
    std::string expected = "/FakeU235/energy/294K";
    EXPECT_EQ(expected, interface_->GetEnergyPath("294K"));
  }

  TEST_F(MaterialsXSFileInterface, Get1DDatasetSize) {

    std::string energy = interface_->GetEnergyPath("294K");
    size_t size;
    ASSERT_NO_THROW(size = interface_->Get1DDatasetSize(energy));
    EXPECT_GE(size, 0);

    EXPECT_THROW(interface_->Get1DDatasetSize("not_real_path"), std::runtime_error);
  }

  TEST_F(MaterialsXSFileInterface, Load1DXSDataset) {
    std::vector<double> energies;
    ASSERT_NO_THROW(interface_->LoadEvaluationEnergies("294K", energies));
    std::vector<float> xs;
    ASSERT_NO_THROW(interface_->Load1DXSDataset("002", "294K", xs, energies.size()));
    EXPECT_FALSE(xs.empty());
    EXPECT_EQ(xs.size(), energies.size());

    // wrong target size
    EXPECT_THROW(interface_->Load1DXSDataset("002", "294K", xs, 0), std::runtime_error);
    // invalid dataset path
    EXPECT_THROW(interface_->Load1DXSDataset("not", "real", xs, energies.size()), std::runtime_error);
    // invalid formatted dataset (char instead of float in this case)
    XSFileInterface bad_file("BadFakeU235");
    EXPECT_THROW(bad_file.Load1DXSDataset("002", "294K", xs, energies.size()), std::runtime_error);
  }

  TEST_F(MaterialsXSFileInterface, LeftPadLoad1DXSDataset) {
    std::vector<double> energies;
    ASSERT_NO_THROW(interface_->LoadEvaluationEnergies("294K", energies));

    std::vector<float> xs_real;
    ASSERT_NO_THROW(interface_->Load1DXSDataset("002", "294K", xs_real, energies.size()));

    std::vector<float> xs_padded;
    ASSERT_NO_THROW(interface_->LeftPadLoad1DXSDataset("002", "294K", xs_padded, energies.size() + 1));
    EXPECT_FALSE(xs_padded.empty());
    EXPECT_EQ(xs_padded.size(), energies.size() + 1);
    EXPECT_DOUBLE_EQ(xs_padded.front(), 0.0);
    for (size_t i = 0; i < xs_real.size(); i++)
    {
      ASSERT_DOUBLE_EQ(xs_padded.at(i+1), xs_real.at(i));
    }
    
    // wrong target size
    EXPECT_THROW(interface_->LeftPadLoad1DXSDataset("002", "294K", xs_padded, energies.size() - 1), std::runtime_error);
    // invalid dataset path
    EXPECT_THROW(interface_->LeftPadLoad1DXSDataset("not", "real", xs_padded, energies.size() + 1), std::runtime_error);
    // invalid formatted dataset (char instead of float in this case)
    XSFileInterface bad_file("BadFakeU235");
    EXPECT_THROW(bad_file.LeftPadLoad1DXSDataset("002", "294K", xs_padded, energies.size() + 1), std::runtime_error);
  }

  TEST_F(MaterialsXSFileInterface, Get1DXSDataPath) {
    std::string expected = "/FakeU235/reactions/reaction_002/294K/xs";
    EXPECT_EQ(expected, interface_->Get1DXSDataPath("002", "294K"));
  }
} // namespace charmander
