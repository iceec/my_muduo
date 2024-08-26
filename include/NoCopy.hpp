#pragma once

class NoCopy
{

    // 删掉这一个 四大天王都没了
NoCopy &operator=(NoCopy &&) = delete;
protected:
    NoCopy() = default;
};