#include "algorithm/sha.hpp"

namespace cn {

public ref class sha256 {
public:
    sha256()
        : hash_{ new ciao::sha256() }
    {}

    void update(cli::array<Byte>^ data)
    {
        cli::pin_ptr<Byte> ptr = &data[0];
        hash_->update(ptr, data->Length);
    }
    cli::array<Byte>^ finalize()
    {
        cli::array<Byte>^ dest = gcnew cli::array<Byte>(256);
        finalize(dest);
        return dest;
    }
    void finalize(cli::array<Byte>^ dest){
        assert(dest->Length > 256);
        cli::pin_ptr<Byte> ptr = &dest[0];
        hash_->finalize(ptr);
    }

private:
    ciao::sha256* hash_;
};
public ref class sha512 {
public:
    sha512()
        : hash_{ new ciao::sha512() }
    {}

    void update(cli::array<Byte>^ data)
    {
        cli::pin_ptr<Byte> ptr = &data[0];
        hash_->update(ptr, data->Length);
    }
    cli::array<Byte>^ finalize()
    {
        cli::array<Byte>^ dest = gcnew cli::array<Byte>(512);
        finalize(dest);
        return dest;
    }
    void finalize(cli::array<Byte>^ dest){
        assert(dest->Length > 512);
        cli::pin_ptr<Byte> ptr = &dest[0];
        hash_->finalize(ptr);
    }

private:
    ciao::sha512* hash_;
};
}

