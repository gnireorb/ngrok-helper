#include "common.hpp"

namespace util
{
	static bool file_exists(const std::filesystem::path& p)
	{
		std::error_code ec;
		return std::filesystem::exists(p, ec);
	}

    static bool to_clipboard(std::string_view str)
    {
        const std::size_t bytes = str.size() + 1;

        HGLOBAL hglobal = GlobalAlloc(GMEM_MOVEABLE, bytes);
        if (!hglobal)
            return false;

        if (void* dst = GlobalLock(hglobal))
        {
            std::memcpy(dst, str.data(), str.size());
            static_cast<char*>(dst)[str.size()] = '\0';
            GlobalUnlock(hglobal);
        }
        else
        {
            GlobalFree(hglobal);
            return false;
        }

        if (!OpenClipboard(nullptr))
        {
            GlobalFree(hglobal);
            return false;
        }

        bool ok = false;
        if (EmptyClipboard() && SetClipboardData(CF_TEXT, hglobal))
            ok = true;

        CloseClipboard();

        if (!ok)
            GlobalFree(hglobal);

        return ok;
    }
}