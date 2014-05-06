/** 
 * @file  OptionsSyntaxColors.cpp
 *
 * @brief Implementation for OptionsSyntaxColors class.
 */
// ID line follows -- this is updated by SVN
// $Id: OptionsSyntaxColors.cpp 6727 2009-05-10 08:25:18Z kimmov $

#include "OptionsSyntaxColors.h"
#include <vector>
#include "SyntaxColors.h"
#include "MergeApp.h"
#include "UnicodeString.h"
#include "OptionsDef.h"
#include "OptionsMgr.h"

using std::vector;

/** @brief Setting name for default colors. */
static const TCHAR DefColorsPath[] =_T("DefaultSyntaxColors");

namespace Options { namespace SyntaxColors {

/**
 * @brief Load color values from storage
 * @param [out] pSyntaxColors pointer to SyntaxColors
 */
void Load(::SyntaxColors *pSyntaxColors)
{
	COptionsMgr *pOptionsMgr = GetOptionsMgr();
	String valuename(DefColorsPath);

	int count = COLORINDEX_COUNT;
	valuename += _T("/Values");
	pOptionsMgr->InitOption(valuename, count);

	for (unsigned i = COLORINDEX_NONE; i < COLORINDEX_LAST; i++)
	{
		// Since we want to initialize with default colors (already
		// set to array, we must first call OptionsMrg->InitOption()
		// with default value. And since InitOption() reads stored value
		// from storage we must set that valu1Ge to array we use.
		int color = 0;
		COLORREF ref;
		color = pSyntaxColors->GetColor(i);

		// Special handling for themable colors
		// These are text colors which by default follow the current system colors
		// unless the user has overridden this behavior to specify them explicitly
		bool serializable = true;
		if (pSyntaxColors->IsThemeableColorIndex(i))
		{
			if (pOptionsMgr->GetBool(OPT_CLR_DEFAULT_TEXT_COLORING))
				serializable = false;
		}
		valuename = string_format(_T("%s/Color%02u"), DefColorsPath, i);
		pOptionsMgr->InitOption(valuename, color, serializable);
		color = pOptionsMgr->GetInt(valuename);
		ref = color;
		pSyntaxColors->SetColor(i, ref);
	
		valuename = string_format(_T("%s/Bold%02u"), DefColorsPath, i);
		bool bBold = pSyntaxColors->GetBold(i);
		pOptionsMgr->InitOption(valuename, bBold);
		int nBold = pOptionsMgr->GetBool(valuename);
		bBold = nBold ? true : false;
		pSyntaxColors->SetBold(i, bBold);
	}
}

/**
 * @brief Save color values to storage
 * @param [in] pSyntaxColors pointer to SyntaxColors
 */
void Save(const ::SyntaxColors *pSyntaxColors)
{
	COptionsMgr *pOptionsMgr = GetOptionsMgr();
	String valuename(DefColorsPath);

	int count = COLORINDEX_COUNT;
	valuename += _T("/Values");
	pOptionsMgr->SaveOption(valuename, count);

	for (unsigned i = COLORINDEX_NONE; i < COLORINDEX_LAST; i++)
	{
		int color = pSyntaxColors->GetColor(i);
		pOptionsMgr->SaveOption(string_format(_T("%s/Color%02u"), DefColorsPath, i), color);
		bool bold = pSyntaxColors->GetBold(i);
		pOptionsMgr->SaveOption(string_format(_T("%s/Bold%02u"), DefColorsPath, i), bold);
	}
}

}}
