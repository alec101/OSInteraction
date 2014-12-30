#include "osinteraction.h"

/* 
 * keysym2unicode is the same as http://www.cl.cam.ac.uk/~mgk25/ucs/keysym2ucs.c
 * but the list was updated from http://www.cl.cam.ac.uk/~mgk25/ucs/keysyms.txt
 * 
 * If a newer/updated list exists, the internal list in this file should be
 * updated with it.
 * 
 * 30/01/2014 - Alec
 */


extern osinteraction osi;

#ifdef OS_LINUX
// -keysyms that have no unicode counterpart were ignored

/* copy-paste from http://www.cl.cam.ac.uk/~mgk25/ucs/keysyms.txt :
# 3    Status of this keysym and its Unicode mapping
#
#         .  regular -- This is a regular well-established keysym with
#            a straightforward Unicode equivalent (e.g., any keysym
#            derived from ISO 8859). There can be at most one regular
#            keysym associated with each Unicode character.
#
#         d  duplicate -- This keysym has the same Unicode mapping as
#            another one with status 'regular'. It represents a case
#            where keysyms distinguish between several characters that
#            Unicode has unified into a single one (examples are
#            several APL symbols)
#
#         o  obsolete -- While it may be possible to find a Unicode of
#            similar name, the exact semantics of this keysym are
#            unclear, because the font or character set from which it
#            came has never been widely used. Examples are various
#            symbols from the DEC Publishing character set, which may
#            have been used in a special font shipped with the
#            DECwrite product. Where no similar Unicode character
#            can be identified, U0000 is used in column 2.
#
#         f  function -- While it may be possible to find a Unicode
#            of similar name, this keysym differs semantically
#            substantially from the corresponding Unicode character,
#            because it describes a particular function key or will
#            first have to be processed by an input method that will
#            translate it into a proper stream of Unicode characters.
#
#         r  remove -- This is a bogus keysym that was added in error,
#            is not used in any known keyboard layout, and should be
#            removed from both <X11/keysymdef.h> and the standard.
#
#         u  unicode-remap -- This keysym was added rather recently to
#            the <X11/keysymdef.h> of XFree86, but into a number range
#            reserved for future extensions of the standard by
#            X.Org. It is not widely used at present, but its name
#            appears to be sufficiently useful and it should therefore
#            be directly mapped to Unicode in the 0x1xxxxxx range in
#            future versions of <X11/keysymdef.h>. This way, the macro
#            name will be preserved, but the standard will not have to
#            be extended.
#
#      Recommendations for using the keysym status:
#
#        - All keysyms with status regular, duplicate, obsolete and
#          function should be listed in Appendix A of the X11 protocol
#          spec.
#
#        - All keysyms except for those with status remove should be
#          listed in <X11/keysymdef.h>.
#
#        - Keysyms with status duplicate, obsolete, and remove should
#          not be used in future keyboard layouts, as there are other
#          keysyms with status regular, function and unicode-remap
#          that give access to the same Unicode characters.
#
#        - Keysym to Unicode conversion tables in clients should include
#          all mappings except those with status function and those
#          with U0000.
*/
struct CodePair {
  uint16 keysym;
  uint16 unicode;
} ks2unicode[]= {
{ 0x01a1, 0x0104 }, // [.] Aogonek
{ 0x01a2, 0x02d8 }, // [.] breve
{ 0x01a3, 0x0141 }, // [.] Lstroke
{ 0x01a5, 0x013d }, // [.] Lcaron
{ 0x01a6, 0x015a }, // [.] Sacute
{ 0x01a9, 0x0160 }, // [.] Scaron
{ 0x01aa, 0x015e }, // [.] Scedilla
{ 0x01ab, 0x0164 }, // [.] Tcaron
{ 0x01ac, 0x0179 }, // [.] Zacute
{ 0x01ae, 0x017d }, // [.] Zcaron
{ 0x01af, 0x017b }, // [.] Zabovedot
{ 0x01b1, 0x0105 }, // [.] aogonek
{ 0x01b2, 0x02db }, // [.] ogonek
{ 0x01b3, 0x0142 }, // [.] lstroke
{ 0x01b5, 0x013e }, // [.] lcaron
{ 0x01b6, 0x015b }, // [.] sacute
{ 0x01b7, 0x02c7 }, // [.] caron
{ 0x01b9, 0x0161 }, // [.] scaron
{ 0x01ba, 0x015f }, // [.] scedilla
{ 0x01bb, 0x0165 }, // [.] tcaron
{ 0x01bc, 0x017a }, // [.] zacute
{ 0x01bd, 0x02dd }, // [.] doubleacute
{ 0x01be, 0x017e }, // [.] zcaron
{ 0x01bf, 0x017c }, // [.] zabovedot
{ 0x01c0, 0x0154 }, // [.] Racute
{ 0x01c3, 0x0102 }, // [.] Abreve
{ 0x01c5, 0x0139 }, // [.] Lacute
{ 0x01c6, 0x0106 }, // [.] Cacute
{ 0x01c8, 0x010c }, // [.] Ccaron
{ 0x01ca, 0x0118 }, // [.] Eogonek
{ 0x01cc, 0x011a }, // [.] Ecaron
{ 0x01cf, 0x010e }, // [.] Dcaron
{ 0x01d0, 0x0110 }, // [.] Dstroke
{ 0x01d1, 0x0143 }, // [.] Nacute
{ 0x01d2, 0x0147 }, // [.] Ncaron
{ 0x01d5, 0x0150 }, // [.] Odoubleacute
{ 0x01d8, 0x0158 }, // [.] Rcaron
{ 0x01d9, 0x016e }, // [.] Uring
{ 0x01db, 0x0170 }, // [.] Udoubleacute
{ 0x01de, 0x0162 }, // [.] Tcedilla
{ 0x01e0, 0x0155 }, // [.] racute
{ 0x01e3, 0x0103 }, // [.] abreve
{ 0x01e5, 0x013a }, // [.] lacute
{ 0x01e6, 0x0107 }, // [.] cacute
{ 0x01e8, 0x010d }, // [.] ccaron
{ 0x01ea, 0x0119 }, // [.] eogonek
{ 0x01ec, 0x011b }, // [.] ecaron
{ 0x01ef, 0x010f }, // [.] dcaron
{ 0x01f0, 0x0111 }, // [.] dstroke
{ 0x01f1, 0x0144 }, // [.] nacute
{ 0x01f2, 0x0148 }, // [.] ncaron
{ 0x01f5, 0x0151 }, // [.] odoubleacute
{ 0x01f8, 0x0159 }, // [.] rcaron
{ 0x01f9, 0x016f }, // [.] uring
{ 0x01fb, 0x0171 }, // [.] udoubleacute
{ 0x01fe, 0x0163 }, // [.] tcedilla
{ 0x01ff, 0x02d9 }, // [.] abovedot
{ 0x02a1, 0x0126 }, // [.] Hstroke
{ 0x02a6, 0x0124 }, // [.] Hcircumflex
{ 0x02a9, 0x0130 }, // [.] Iabovedot
{ 0x02ab, 0x011e }, // [.] Gbreve
{ 0x02ac, 0x0134 }, // [.] Jcircumflex
{ 0x02b1, 0x0127 }, // [.] hstroke
{ 0x02b6, 0x0125 }, // [.] hcircumflex
{ 0x02b9, 0x0131 }, // [.] idotless
{ 0x02bb, 0x011f }, // [.] gbreve
{ 0x02bc, 0x0135 }, // [.] jcircumflex
{ 0x02c5, 0x010a }, // [.] Cabovedot
{ 0x02c6, 0x0108 }, // [.] Ccircumflex
{ 0x02d5, 0x0120 }, // [.] Gabovedot
{ 0x02d8, 0x011c }, // [.] Gcircumflex
{ 0x02dd, 0x016c }, // [.] Ubreve
{ 0x02de, 0x015c }, // [.] Scircumflex
{ 0x02e5, 0x010b }, // [.] cabovedot
{ 0x02e6, 0x0109 }, // [.] ccircumflex
{ 0x02f5, 0x0121 }, // [.] gabovedot
{ 0x02f8, 0x011d }, // [.] gcircumflex
{ 0x02fd, 0x016d }, // [.] ubreve
{ 0x02fe, 0x015d }, // [.] scircumflex
{ 0x03a2, 0x0138 }, // [.] kra
{ 0x03a3, 0x0156 }, // [.] Rcedilla
{ 0x03a5, 0x0128 }, // [.] Itilde
{ 0x03a6, 0x013b }, // [.] Lcedilla
{ 0x03aa, 0x0112 }, // [.] Emacron
{ 0x03ab, 0x0122 }, // [.] Gcedilla
{ 0x03ac, 0x0166 }, // [.] Tslash
{ 0x03b3, 0x0157 }, // [.] rcedilla
{ 0x03b5, 0x0129 }, // [.] itilde
{ 0x03b6, 0x013c }, // [.] lcedilla
{ 0x03ba, 0x0113 }, // [.] emacron
{ 0x03bb, 0x0123 }, // [.] gcedilla
{ 0x03bc, 0x0167 }, // [.] tslash
{ 0x03bd, 0x014a }, // [.] ENG
{ 0x03bf, 0x014b }, // [.] eng
{ 0x03c0, 0x0100 }, // [.] Amacron
{ 0x03c7, 0x012e }, // [.] Iogonek
{ 0x03cc, 0x0116 }, // [.] Eabovedot
{ 0x03cf, 0x012a }, // [.] Imacron
{ 0x03d1, 0x0145 }, // [.] Ncedilla
{ 0x03d2, 0x014c }, // [.] Omacron
{ 0x03d3, 0x0136 }, // [.] Kcedilla
{ 0x03d9, 0x0172 }, // [.] Uogonek
{ 0x03dd, 0x0168 }, // [.] Utilde
{ 0x03de, 0x016a }, // [.] Umacron
{ 0x03e0, 0x0101 }, // [.] amacron
{ 0x03e7, 0x012f }, // [.] iogonek
{ 0x03ec, 0x0117 }, // [.] eabovedot
{ 0x03ef, 0x012b }, // [.] imacron
{ 0x03f1, 0x0146 }, // [.] ncedilla
{ 0x03f2, 0x014d }, // [.] omacron
{ 0x03f3, 0x0137 }, // [.] kcedilla
{ 0x03f9, 0x0173 }, // [.] uogonek
{ 0x03fd, 0x0169 }, // [.] utilde
{ 0x03fe, 0x016b }, // [.] umacron
{ 0x047e, 0x203e }, // [.] overline
{ 0x04a1, 0x3002 }, // [.] kana_fullstop
{ 0x04a2, 0x300c }, // [.] kana_openingbracket
{ 0x04a3, 0x300d }, // [.] kana_closingbracket
{ 0x04a4, 0x3001 }, // [.] kana_comma
{ 0x04a5, 0x30fb }, // [.] kana_conjunctive
{ 0x04a6, 0x30f2 }, // [.] kana_WO
{ 0x04a7, 0x30a1 }, // [.] kana_a
{ 0x04a8, 0x30a3 }, // [.] kana_i
{ 0x04a9, 0x30a5 }, // [.] kana_u
{ 0x04aa, 0x30a7 }, // [.] kana_e
{ 0x04ab, 0x30a9 }, // [.] kana_o
{ 0x04ac, 0x30e3 }, // [.] kana_ya
{ 0x04ad, 0x30e5 }, // [.] kana_yu
{ 0x04ae, 0x30e7 }, // [.] kana_yo
{ 0x04af, 0x30c3 }, // [.] kana_tsu
{ 0x04b0, 0x30fc }, // [.] prolongedsound
{ 0x04b1, 0x30a2 }, // [.] kana_A
{ 0x04b2, 0x30a4 }, // [.] kana_I
{ 0x04b3, 0x30a6 }, // [.] kana_U
{ 0x04b4, 0x30a8 }, // [.] kana_E
{ 0x04b5, 0x30aa }, // [.] kana_O
{ 0x04b6, 0x30ab }, // [.] kana_KA
{ 0x04b7, 0x30ad }, // [.] kana_KI
{ 0x04b8, 0x30af }, // [.] kana_KU
{ 0x04b9, 0x30b1 }, // [.] kana_KE
{ 0x04ba, 0x30b3 }, // [.] kana_KO
{ 0x04bb, 0x30b5 }, // [.] kana_SA
{ 0x04bc, 0x30b7 }, // [.] kana_SHI
{ 0x04bd, 0x30b9 }, // [.] kana_SU
{ 0x04be, 0x30bb }, // [.] kana_SE
{ 0x04bf, 0x30bd }, // [.] kana_SO
{ 0x04c0, 0x30bf }, // [.] kana_TA
{ 0x04c1, 0x30c1 }, // [.] kana_CHI
{ 0x04c2, 0x30c4 }, // [.] kana_TSU
{ 0x04c3, 0x30c6 }, // [.] kana_TE
{ 0x04c4, 0x30c8 }, // [.] kana_TO
{ 0x04c5, 0x30ca }, // [.] kana_NA
{ 0x04c6, 0x30cb }, // [.] kana_NI
{ 0x04c7, 0x30cc }, // [.] kana_NU
{ 0x04c8, 0x30cd }, // [.] kana_NE
{ 0x04c9, 0x30ce }, // [.] kana_NO
{ 0x04ca, 0x30cf }, // [.] kana_HA
{ 0x04cb, 0x30d2 }, // [.] kana_HI
{ 0x04cc, 0x30d5 }, // [.] kana_FU
{ 0x04cd, 0x30d8 }, // [.] kana_HE
{ 0x04ce, 0x30db }, // [.] kana_HO
{ 0x04cf, 0x30de }, // [.] kana_MA
{ 0x04d0, 0x30df }, // [.] kana_MI
{ 0x04d1, 0x30e0 }, // [.] kana_MU
{ 0x04d2, 0x30e1 }, // [.] kana_ME
{ 0x04d3, 0x30e2 }, // [.] kana_MO
{ 0x04d4, 0x30e4 }, // [.] kana_YA
{ 0x04d5, 0x30e6 }, // [.] kana_YU
{ 0x04d6, 0x30e8 }, // [.] kana_YO
{ 0x04d7, 0x30e9 }, // [.] kana_RA
{ 0x04d8, 0x30ea }, // [.] kana_RI
{ 0x04d9, 0x30eb }, // [.] kana_RU
{ 0x04da, 0x30ec }, // [.] kana_RE
{ 0x04db, 0x30ed }, // [.] kana_RO
{ 0x04dc, 0x30ef }, // [.] kana_WA
{ 0x04dd, 0x30f3 }, // [.] kana_N
{ 0x04de, 0x309b }, // [.] voicedsound
{ 0x04df, 0x309c }, // [.] semivoicedsound
{ 0x0590, 0x06f0 }, // [u] Farsi_0
{ 0x0591, 0x06f1 }, // [u] Farsi_1
{ 0x0592, 0x06f2 }, // [u] Farsi_2
{ 0x0593, 0x06f3 }, // [u] Farsi_3
{ 0x0594, 0x06f4 }, // [u] Farsi_4
{ 0x0595, 0x06f5 }, // [u] Farsi_5
{ 0x0596, 0x06f6 }, // [u] Farsi_6
{ 0x0597, 0x06f7 }, // [u] Farsi_7
{ 0x0598, 0x06f8 }, // [u] Farsi_8
{ 0x0599, 0x06f9 }, // [u] Farsi_9
{ 0x05a5, 0x066a }, // [u] Arabic_percent
{ 0x05a6, 0x0670 }, // [u] Arabic_superscript_alef
{ 0x05a7, 0x0679 }, // [u] Arabic_tteh
{ 0x05a8, 0x067e }, // [u] Arabic_peh
{ 0x05a9, 0x0686 }, // [u] Arabic_tcheh
{ 0x05aa, 0x0688 }, // [u] Arabic_ddal
{ 0x05ab, 0x0691 }, // [u] Arabic_rreh
{ 0x05ac, 0x060c }, // [.] Arabic_comma
{ 0x05ae, 0x06d4 }, // [u] Arabic_fullstop
{ 0x05b0, 0x0660 }, // [u] Arabic_0
{ 0x05b1, 0x0661 }, // [u] Arabic_1
{ 0x05b2, 0x0662 }, // [u] Arabic_2
{ 0x05b3, 0x0663 }, // [u] Arabic_3
{ 0x05b4, 0x0664 }, // [u] Arabic_4
{ 0x05b5, 0x0665 }, // [u] Arabic_5
{ 0x05b6, 0x0666 }, // [u] Arabic_6
{ 0x05b7, 0x0667 }, // [u] Arabic_7
{ 0x05b8, 0x0668 }, // [u] Arabic_8
{ 0x05b9, 0x0669 }, // [u] Arabic_9
{ 0x05bb, 0x061b }, // [.] Arabic_semicolon
{ 0x05bf, 0x061f }, // [.] Arabic_question_mark
{ 0x05c1, 0x0621 }, // [.] Arabic_hamza
{ 0x05c2, 0x0622 }, // [.] Arabic_maddaonalef
{ 0x05c3, 0x0623 }, // [.] Arabic_hamzaonalef
{ 0x05c4, 0x0624 }, // [.] Arabic_hamzaonwaw
{ 0x05c5, 0x0625 }, // [.] Arabic_hamzaunderalef
{ 0x05c6, 0x0626 }, // [.] Arabic_hamzaonyeh
{ 0x05c7, 0x0627 }, // [.] Arabic_alef
{ 0x05c8, 0x0628 }, // [.] Arabic_beh
{ 0x05c9, 0x0629 }, // [.] Arabic_tehmarbuta
{ 0x05ca, 0x062a }, // [.] Arabic_teh
{ 0x05cb, 0x062b }, // [.] Arabic_theh
{ 0x05cc, 0x062c }, // [.] Arabic_jeem
{ 0x05cd, 0x062d }, // [.] Arabic_hah
{ 0x05ce, 0x062e }, // [.] Arabic_khah
{ 0x05cf, 0x062f }, // [.] Arabic_dal
{ 0x05d0, 0x0630 }, // [.] Arabic_thal
{ 0x05d1, 0x0631 }, // [.] Arabic_ra
{ 0x05d2, 0x0632 }, // [.] Arabic_zain
{ 0x05d3, 0x0633 }, // [.] Arabic_seen
{ 0x05d4, 0x0634 }, // [.] Arabic_sheen
{ 0x05d5, 0x0635 }, // [.] Arabic_sad
{ 0x05d6, 0x0636 }, // [.] Arabic_dad
{ 0x05d7, 0x0637 }, // [.] Arabic_tah
{ 0x05d8, 0x0638 }, // [.] Arabic_zah
{ 0x05d9, 0x0639 }, // [.] Arabic_ain
{ 0x05da, 0x063a }, // [.] Arabic_ghain
{ 0x05e0, 0x0640 }, // [.] Arabic_tatweel
{ 0x05e1, 0x0641 }, // [.] Arabic_feh
{ 0x05e2, 0x0642 }, // [.] Arabic_qaf
{ 0x05e3, 0x0643 }, // [.] Arabic_kaf
{ 0x05e4, 0x0644 }, // [.] Arabic_lam
{ 0x05e5, 0x0645 }, // [.] Arabic_meem
{ 0x05e6, 0x0646 }, // [.] Arabic_noon
{ 0x05e7, 0x0647 }, // [.] Arabic_ha
{ 0x05e8, 0x0648 }, // [.] Arabic_waw
{ 0x05e9, 0x0649 }, // [.] Arabic_alefmaksura
{ 0x05ea, 0x064a }, // [.] Arabic_yeh
{ 0x05eb, 0x064b }, // [.] Arabic_fathatan
{ 0x05ec, 0x064c }, // [.] Arabic_dammatan
{ 0x05ed, 0x064d }, // [.] Arabic_kasratan
{ 0x05ee, 0x064e }, // [.] Arabic_fatha
{ 0x05ef, 0x064f }, // [.] Arabic_damma
{ 0x05f0, 0x0650 }, // [.] Arabic_kasra
{ 0x05f1, 0x0651 }, // [.] Arabic_shadda
{ 0x05f2, 0x0652 }, // [.] Arabic_sukun
{ 0x05f3, 0x0653 }, // [u] Arabic_madda_above
{ 0x05f4, 0x0654 }, // [u] Arabic_hamza_above
{ 0x05f5, 0x0655 }, // [u] Arabic_hamza_below
{ 0x05f6, 0x0698 }, // [u] Arabic_jeh
{ 0x05f7, 0x06a4 }, // [u] Arabic_veh
{ 0x05f8, 0x06a9 }, // [u] Arabic_keheh
{ 0x05f9, 0x06af }, // [u] Arabic_gaf
{ 0x05fa, 0x06ba }, // [u] Arabic_noon_ghunna
{ 0x05fb, 0x06be }, // [u] Arabic_heh_doachashmee
{ 0x05fc, 0x06cc }, // [u] Farsi_yeh
{ 0x05fd, 0x06d2 }, // [u] Arabic_yeh_baree
{ 0x05fe, 0x06c1 }, // [u] Arabic_heh_goal
{ 0x0680, 0x0492 }, // [u] Cyrillic_GHE_bar
{ 0x0681, 0x0496 }, // [u] Cyrillic_ZHE_descender
{ 0x0682, 0x049a }, // [u] Cyrillic_KA_descender
{ 0x0683, 0x049c }, // [u] Cyrillic_KA_vertstroke
{ 0x0684, 0x04a2 }, // [u] Cyrillic_EN_descender
{ 0x0685, 0x04ae }, // [u] Cyrillic_U_straight
{ 0x0686, 0x04b0 }, // [u] Cyrillic_U_straight_bar
{ 0x0687, 0x04b2 }, // [u] Cyrillic_HA_descender
{ 0x0688, 0x04b6 }, // [u] Cyrillic_CHE_descender
{ 0x0689, 0x04b8 }, // [u] Cyrillic_CHE_vertstroke
{ 0x068a, 0x04ba }, // [u] Cyrillic_SHHA
{ 0x068c, 0x04d8 }, // [u] Cyrillic_SCHWA
{ 0x068d, 0x04e2 }, // [u] Cyrillic_I_macron
{ 0x068e, 0x04e8 }, // [u] Cyrillic_O_bar
{ 0x068f, 0x04ee }, // [u] Cyrillic_U_macron
{ 0x0690, 0x0493 }, // [u] Cyrillic_ghe_bar
{ 0x0691, 0x0497 }, // [u] Cyrillic_zhe_descender
{ 0x0692, 0x049b }, // [u] Cyrillic_ka_descender
{ 0x0693, 0x049d }, // [u] Cyrillic_ka_vertstroke
{ 0x0694, 0x04a3 }, // [u] Cyrillic_en_descender
{ 0x0695, 0x04af }, // [u] Cyrillic_u_straight
{ 0x0696, 0x04b1 }, // [u] Cyrillic_u_straight_bar
{ 0x0697, 0x04b3 }, // [u] Cyrillic_ha_descender
{ 0x0698, 0x04b7 }, // [u] Cyrillic_che_descender
{ 0x0699, 0x04b9 }, // [u] Cyrillic_che_vertstroke
{ 0x069a, 0x04bb }, // [u] Cyrillic_shha
{ 0x069c, 0x04d9 }, // [u] Cyrillic_schwa
{ 0x069d, 0x04e3 }, // [u] Cyrillic_i_macron
{ 0x069e, 0x04e9 }, // [u] Cyrillic_o_bar
{ 0x069f, 0x04ef }, // [u] Cyrillic_u_macron
{ 0x06a1, 0x0452 }, // [.] Serbian_dje
{ 0x06a2, 0x0453 }, // [.] Macedonia_gje
{ 0x06a3, 0x0451 }, // [.] Cyrillic_io
{ 0x06a4, 0x0454 }, // [.] Ukrainian_ie
{ 0x06a5, 0x0455 }, // [.] Macedonia_dse
{ 0x06a6, 0x0456 }, // [.] Ukrainian_i
{ 0x06a7, 0x0457 }, // [.] Ukrainian_yi
{ 0x06a8, 0x0458 }, // [.] Cyrillic_je
{ 0x06a9, 0x0459 }, // [.] Cyrillic_lje
{ 0x06aa, 0x045a }, // [.] Cyrillic_nje
{ 0x06ab, 0x045b }, // [.] Serbian_tshe
{ 0x06ac, 0x045c }, // [.] Macedonia_kje
{ 0x06ad, 0x0491 }, // [.] Ukrainian_ghe_with_upturn
{ 0x06ae, 0x045e }, // [.] Byelorussian_shortu
{ 0x06af, 0x045f }, // [.] Cyrillic_dzhe
{ 0x06b0, 0x2116 }, // [.] numerosign
{ 0x06b1, 0x0402 }, // [.] Serbian_DJE
{ 0x06b2, 0x0403 }, // [.] Macedonia_GJE
{ 0x06b3, 0x0401 }, // [.] Cyrillic_IO
{ 0x06b4, 0x0404 }, // [.] Ukrainian_IE
{ 0x06b5, 0x0405 }, // [.] Macedonia_DSE
{ 0x06b6, 0x0406 }, // [.] Ukrainian_I
{ 0x06b7, 0x0407 }, // [.] Ukrainian_YI
{ 0x06b8, 0x0408 }, // [.] Cyrillic_JE
{ 0x06b9, 0x0409 }, // [.] Cyrillic_LJE
{ 0x06ba, 0x040a }, // [.] Cyrillic_NJE
{ 0x06bb, 0x040b }, // [.] Serbian_TSHE
{ 0x06bc, 0x040c }, // [.] Macedonia_KJE
{ 0x06bd, 0x0490 }, // [.] Ukrainian_GHE_WITH_UPTURN
{ 0x06be, 0x040e }, // [.] Byelorussian_SHORTU
{ 0x06bf, 0x040f }, // [.] Cyrillic_DZHE
{ 0x06c0, 0x044e }, // [.] Cyrillic_yu
{ 0x06c1, 0x0430 }, // [.] Cyrillic_a
{ 0x06c2, 0x0431 }, // [.] Cyrillic_be
{ 0x06c3, 0x0446 }, // [.] Cyrillic_tse
{ 0x06c4, 0x0434 }, // [.] Cyrillic_de
{ 0x06c5, 0x0435 }, // [.] Cyrillic_ie
{ 0x06c6, 0x0444 }, // [.] Cyrillic_ef
{ 0x06c7, 0x0433 }, // [.] Cyrillic_ghe
{ 0x06c8, 0x0445 }, // [.] Cyrillic_ha
{ 0x06c9, 0x0438 }, // [.] Cyrillic_i
{ 0x06ca, 0x0439 }, // [.] Cyrillic_shorti
{ 0x06cb, 0x043a }, // [.] Cyrillic_ka
{ 0x06cc, 0x043b }, // [.] Cyrillic_el
{ 0x06cd, 0x043c }, // [.] Cyrillic_em
{ 0x06ce, 0x043d }, // [.] Cyrillic_en
{ 0x06cf, 0x043e }, // [.] Cyrillic_o
{ 0x06d0, 0x043f }, // [.] Cyrillic_pe
{ 0x06d1, 0x044f }, // [.] Cyrillic_ya
{ 0x06d2, 0x0440 }, // [.] Cyrillic_er
{ 0x06d3, 0x0441 }, // [.] Cyrillic_es
{ 0x06d4, 0x0442 }, // [.] Cyrillic_te
{ 0x06d5, 0x0443 }, // [.] Cyrillic_u
{ 0x06d6, 0x0436 }, // [.] Cyrillic_zhe
{ 0x06d7, 0x0432 }, // [.] Cyrillic_ve
{ 0x06d8, 0x044c }, // [.] Cyrillic_softsign
{ 0x06d9, 0x044b }, // [.] Cyrillic_yeru
{ 0x06da, 0x0437 }, // [.] Cyrillic_ze
{ 0x06db, 0x0448 }, // [.] Cyrillic_sha
{ 0x06dc, 0x044d }, // [.] Cyrillic_e
{ 0x06dd, 0x0449 }, // [.] Cyrillic_shcha
{ 0x06de, 0x0447 }, // [.] Cyrillic_che
{ 0x06df, 0x044a }, // [.] Cyrillic_hardsign
{ 0x06e0, 0x042e }, // [.] Cyrillic_YU
{ 0x06e1, 0x0410 }, // [.] Cyrillic_A
{ 0x06e2, 0x0411 }, // [.] Cyrillic_BE
{ 0x06e3, 0x0426 }, // [.] Cyrillic_TSE
{ 0x06e4, 0x0414 }, // [.] Cyrillic_DE
{ 0x06e5, 0x0415 }, // [.] Cyrillic_IE
{ 0x06e6, 0x0424 }, // [.] Cyrillic_EF
{ 0x06e7, 0x0413 }, // [.] Cyrillic_GHE
{ 0x06e8, 0x0425 }, // [.] Cyrillic_HA
{ 0x06e9, 0x0418 }, // [.] Cyrillic_I
{ 0x06ea, 0x0419 }, // [.] Cyrillic_SHORTI
{ 0x06eb, 0x041a }, // [.] Cyrillic_KA
{ 0x06ec, 0x041b }, // [.] Cyrillic_EL
{ 0x06ed, 0x041c }, // [.] Cyrillic_EM
{ 0x06ee, 0x041d }, // [.] Cyrillic_EN
{ 0x06ef, 0x041e }, // [.] Cyrillic_O
{ 0x06f0, 0x041f }, // [.] Cyrillic_PE
{ 0x06f1, 0x042f }, // [.] Cyrillic_YA
{ 0x06f2, 0x0420 }, // [.] Cyrillic_ER
{ 0x06f3, 0x0421 }, // [.] Cyrillic_ES
{ 0x06f4, 0x0422 }, // [.] Cyrillic_TE
{ 0x06f5, 0x0423 }, // [.] Cyrillic_U
{ 0x06f6, 0x0416 }, // [.] Cyrillic_ZHE
{ 0x06f7, 0x0412 }, // [.] Cyrillic_VE
{ 0x06f8, 0x042c }, // [.] Cyrillic_SOFTSIGN
{ 0x06f9, 0x042b }, // [.] Cyrillic_YERU
{ 0x06fa, 0x0417 }, // [.] Cyrillic_ZE
{ 0x06fb, 0x0428 }, // [.] Cyrillic_SHA
{ 0x06fc, 0x042d }, // [.] Cyrillic_E
{ 0x06fd, 0x0429 }, // [.] Cyrillic_SHCHA
{ 0x06fe, 0x0427 }, // [.] Cyrillic_CHE
{ 0x06ff, 0x042a }, // [.] Cyrillic_HARDSIGN
{ 0x07a1, 0x0386 }, // [.] Greek_ALPHAaccent
{ 0x07a2, 0x0388 }, // [.] Greek_EPSILONaccent
{ 0x07a3, 0x0389 }, // [.] Greek_ETAaccent
{ 0x07a4, 0x038a }, // [.] Greek_IOTAaccent
{ 0x07a5, 0x03aa }, // [.] Greek_IOTAdiaeresis
{ 0x07a7, 0x038c }, // [.] Greek_OMICRONaccent
{ 0x07a8, 0x038e }, // [.] Greek_UPSILONaccent
{ 0x07a9, 0x03ab }, // [.] Greek_UPSILONdieresis
{ 0x07ab, 0x038f }, // [.] Greek_OMEGAaccent
{ 0x07ae, 0x0385 }, // [.] Greek_accentdieresis
{ 0x07af, 0x2015 }, // [.] Greek_horizbar
{ 0x07b1, 0x03ac }, // [.] Greek_alphaaccent
{ 0x07b2, 0x03ad }, // [.] Greek_epsilonaccent
{ 0x07b3, 0x03ae }, // [.] Greek_etaaccent
{ 0x07b4, 0x03af }, // [.] Greek_iotaaccent
{ 0x07b5, 0x03ca }, // [.] Greek_iotadieresis
{ 0x07b6, 0x0390 }, // [.] Greek_iotaaccentdieresis
{ 0x07b7, 0x03cc }, // [.] Greek_omicronaccent
{ 0x07b8, 0x03cd }, // [.] Greek_upsilonaccent
{ 0x07b9, 0x03cb }, // [.] Greek_upsilondieresis
{ 0x07ba, 0x03b0 }, // [.] Greek_upsilonaccentdieresis
{ 0x07bb, 0x03ce }, // [.] Greek_omegaaccent
{ 0x07c1, 0x0391 }, // [.] Greek_ALPHA
{ 0x07c2, 0x0392 }, // [.] Greek_BETA
{ 0x07c3, 0x0393 }, // [.] Greek_GAMMA
{ 0x07c4, 0x0394 }, // [.] Greek_DELTA
{ 0x07c5, 0x0395 }, // [.] Greek_EPSILON
{ 0x07c6, 0x0396 }, // [.] Greek_ZETA
{ 0x07c7, 0x0397 }, // [.] Greek_ETA
{ 0x07c8, 0x0398 }, // [.] Greek_THETA
{ 0x07c9, 0x0399 }, // [.] Greek_IOTA
{ 0x07ca, 0x039a }, // [.] Greek_KAPPA
{ 0x07cb, 0x039b }, // [.] Greek_LAMBDA
{ 0x07cb, 0x039b }, // [.] Greek_LAMDA
{ 0x07cc, 0x039c }, // [.] Greek_MU
{ 0x07cd, 0x039d }, // [.] Greek_NU
{ 0x07ce, 0x039e }, // [.] Greek_XI
{ 0x07cf, 0x039f }, // [.] Greek_OMICRON
{ 0x07d0, 0x03a0 }, // [.] Greek_PI
{ 0x07d1, 0x03a1 }, // [.] Greek_RHO
{ 0x07d2, 0x03a3 }, // [.] Greek_SIGMA
{ 0x07d4, 0x03a4 }, // [.] Greek_TAU
{ 0x07d5, 0x03a5 }, // [.] Greek_UPSILON
{ 0x07d6, 0x03a6 }, // [.] Greek_PHI
{ 0x07d7, 0x03a7 }, // [.] Greek_CHI
{ 0x07d8, 0x03a8 }, // [.] Greek_PSI
{ 0x07d9, 0x03a9 }, // [.] Greek_OMEGA
{ 0x07e1, 0x03b1 }, // [.] Greek_alpha
{ 0x07e2, 0x03b2 }, // [.] Greek_beta
{ 0x07e3, 0x03b3 }, // [.] Greek_gamma
{ 0x07e4, 0x03b4 }, // [.] Greek_delta
{ 0x07e5, 0x03b5 }, // [.] Greek_epsilon
{ 0x07e6, 0x03b6 }, // [.] Greek_zeta
{ 0x07e7, 0x03b7 }, // [.] Greek_eta
{ 0x07e8, 0x03b8 }, // [.] Greek_theta
{ 0x07e9, 0x03b9 }, // [.] Greek_iota
{ 0x07ea, 0x03ba }, // [.] Greek_kappa
{ 0x07eb, 0x03bb }, // [.] Greek_lambda
{ 0x07ec, 0x03bc }, // [.] Greek_mu
{ 0x07ed, 0x03bd }, // [.] Greek_nu
{ 0x07ee, 0x03be }, // [.] Greek_xi
{ 0x07ef, 0x03bf }, // [.] Greek_omicron
{ 0x07f0, 0x03c0 }, // [.] Greek_pi
{ 0x07f1, 0x03c1 }, // [.] Greek_rho
{ 0x07f2, 0x03c3 }, // [.] Greek_sigma
{ 0x07f3, 0x03c2 }, // [.] Greek_finalsmallsigma
{ 0x07f4, 0x03c4 }, // [.] Greek_tau
{ 0x07f5, 0x03c5 }, // [.] Greek_upsilon
{ 0x07f6, 0x03c6 }, // [.] Greek_phi
{ 0x07f7, 0x03c7 }, // [.] Greek_chi
{ 0x07f8, 0x03c8 }, // [.] Greek_psi
{ 0x07f9, 0x03c9 }, // [.] Greek_omega
{ 0x08a1, 0x23b7 }, // [.] leftradical
{ 0x08a2, 0x250c }, // [d] topleftradical
{ 0x08a3, 0x2500 }, // [d] horizconnector
{ 0x08a4, 0x2320 }, // [.] topintegral
{ 0x08a5, 0x2321 }, // [.] botintegral
{ 0x08a6, 0x2502 }, // [d] vertconnector
{ 0x08a7, 0x23a1 }, // [.] topleftsqbracket
{ 0x08a8, 0x23a3 }, // [.] botleftsqbracket
{ 0x08a9, 0x23a4 }, // [.] toprightsqbracket
{ 0x08aa, 0x23a6 }, // [.] botrightsqbracket
{ 0x08ab, 0x239b }, // [.] topleftparens
{ 0x08ac, 0x239d }, // [.] botleftparens
{ 0x08ad, 0x239e }, // [.] toprightparens
{ 0x08ae, 0x23a0 }, // [.] botrightparens
{ 0x08af, 0x23a8 }, // [.] leftmiddlecurlybrace
{ 0x08b0, 0x23ac }, // [.] rightmiddlecurlybrace
{ 0x08bc, 0x2264 }, // [.] lessthanequal
{ 0x08bd, 0x2260 }, // [.] notequal
{ 0x08be, 0x2265 }, // [.] greaterthanequal
{ 0x08bf, 0x222b }, // [.] integral
{ 0x08c0, 0x2234 }, // [.] therefore
{ 0x08c1, 0x221d }, // [.] variation
{ 0x08c2, 0x221e }, // [.] infinity
{ 0x08c5, 0x2207 }, // [.] nabla
{ 0x08c8, 0x223c }, // [.] approximate
{ 0x08c9, 0x2243 }, // [.] similarequal
{ 0x08cd, 0x21d4 }, // [.] ifonlyif
{ 0x08ce, 0x21d2 }, // [.] implies
{ 0x08cf, 0x2261 }, // [.] identical
{ 0x08d6, 0x221a }, // [.] radical
{ 0x08da, 0x2282 }, // [.] includedin
{ 0x08db, 0x2283 }, // [.] includes
{ 0x08dc, 0x2229 }, // [.] intersection
{ 0x08dd, 0x222a }, // [.] union
{ 0x08de, 0x2227 }, // [.] logicaland
{ 0x08df, 0x2228 }, // [.] logicalor
{ 0x08ef, 0x2202 }, // [.] partialderivative
{ 0x08f6, 0x0192 }, // [.] function
{ 0x08fb, 0x2190 }, // [.] leftarrow
{ 0x08fc, 0x2191 }, // [.] uparrow
{ 0x08fd, 0x2192 }, // [.] rightarrow
{ 0x08fe, 0x2193 }, // [.] downarrow
{ 0x09e0, 0x25c6 }, // [.] soliddiamond
{ 0x09e1, 0x2592 }, // [.] checkerboard
{ 0x09e2, 0x2409 }, // [.] ht
{ 0x09e3, 0x240c }, // [.] ff
{ 0x09e4, 0x240d }, // [.] cr
{ 0x09e5, 0x240a }, // [.] lf
{ 0x09e8, 0x2424 }, // [.] nl
{ 0x09e9, 0x240b }, // [.] vt
{ 0x09ea, 0x2518 }, // [.] lowrightcorner
{ 0x09eb, 0x2510 }, // [.] uprightcorner
{ 0x09ec, 0x250c }, // [.] upleftcorner
{ 0x09ed, 0x2514 }, // [.] lowleftcorner
{ 0x09ee, 0x253c }, // [.] crossinglines
{ 0x09ef, 0x23ba }, // [.] horizlinescan1
{ 0x09f0, 0x23bb }, // [.] horizlinescan3
{ 0x09f1, 0x2500 }, // [.] horizlinescan5
{ 0x09f2, 0x23bc }, // [.] horizlinescan7
{ 0x09f3, 0x23bd }, // [.] horizlinescan9
{ 0x09f4, 0x251c }, // [.] leftt
{ 0x09f5, 0x2524 }, // [.] rightt
{ 0x09f6, 0x2534 }, // [.] bott
{ 0x09f7, 0x252c }, // [.] topt
{ 0x09f8, 0x2502 }, // [.] vertbar
{ 0x0aa1, 0x2003 }, // [.] emspace
{ 0x0aa2, 0x2002 }, // [.] enspace
{ 0x0aa3, 0x2004 }, // [.] em3space
{ 0x0aa4, 0x2005 }, // [.] em4space
{ 0x0aa5, 0x2007 }, // [.] digitspace
{ 0x0aa6, 0x2008 }, // [.] punctspace
{ 0x0aa7, 0x2009 }, // [.] thinspace
{ 0x0aa8, 0x200a }, // [.] hairspace
{ 0x0aa9, 0x2014 }, // [.] emdash
{ 0x0aaa, 0x2013 }, // [.] endash
{ 0x0aac, 0x2423 }, // [o] signifblank
{ 0x0aae, 0x2026 }, // [.] ellipsis
{ 0x0aaf, 0x2025 }, // [.] doubbaselinedot
{ 0x0ab0, 0x2153 }, // [.] onethird
{ 0x0ab1, 0x2154 }, // [.] twothirds
{ 0x0ab2, 0x2155 }, // [.] onefifth
{ 0x0ab3, 0x2156 }, // [.] twofifths
{ 0x0ab4, 0x2157 }, // [.] threefifths
{ 0x0ab5, 0x2158 }, // [.] fourfifths
{ 0x0ab6, 0x2159 }, // [.] onesixth
{ 0x0ab7, 0x215a }, // [.] fivesixths
{ 0x0ab8, 0x2105 }, // [.] careof
{ 0x0abb, 0x2012 }, // [.] figdash
{ 0x0abc, 0x27e8 }, // [o] leftanglebracket
{ 0x0abd, 0x002e }, // [o] decimalpoint
{ 0x0abe, 0x27e9 }, // [o] rightanglebracket
{ 0x0ac3, 0x215b }, // [.] oneeighth
{ 0x0ac4, 0x215c }, // [.] threeeighths
{ 0x0ac5, 0x215d }, // [.] fiveeighths
{ 0x0ac6, 0x215e }, // [.] seveneighths
{ 0x0ac9, 0x2122 }, // [.] trademark
{ 0x0aca, 0x2613 }, // [o] signaturemark
{ 0x0acc, 0x25c1 }, // [o] leftopentriangle
{ 0x0acd, 0x25b7 }, // [o] rightopentriangle
{ 0x0ace, 0x25cb }, // [o] emopencircle
{ 0x0acf, 0x25af }, // [o] emopenrectangle
{ 0x0ad0, 0x2018 }, // [.] leftsinglequotemark
{ 0x0ad1, 0x2019 }, // [.] rightsinglequotemark
{ 0x0ad2, 0x201c }, // [.] leftdoublequotemark
{ 0x0ad3, 0x201d }, // [.] rightdoublequotemark
{ 0x0ad4, 0x211e }, // [.] prescription
{ 0x0ad6, 0x2032 }, // [.] minutes
{ 0x0ad7, 0x2033 }, // [.] seconds
{ 0x0ad9, 0x271d }, // [.] latincross
{ 0x0adb, 0x25ac }, // [o] filledrectbullet
{ 0x0adc, 0x25c0 }, // [o] filledlefttribullet
{ 0x0add, 0x25b6 }, // [o] filledrighttribullet
{ 0x0ade, 0x25cf }, // [o] emfilledcircle
{ 0x0adf, 0x25ae }, // [o] emfilledrect
{ 0x0ae0, 0x25e6 }, // [o] enopencircbullet
{ 0x0ae1, 0x25ab }, // [o] enopensquarebullet
{ 0x0ae2, 0x25ad }, // [o] openrectbullet
{ 0x0ae3, 0x25b3 }, // [o] opentribulletup
{ 0x0ae4, 0x25bd }, // [o] opentribulletdown
{ 0x0ae5, 0x2606 }, // [o] openstar
{ 0x0ae6, 0x2022 }, // [o] enfilledcircbullet
{ 0x0ae7, 0x25aa }, // [o] enfilledsqbullet
{ 0x0ae8, 0x25b2 }, // [o] filledtribulletup
{ 0x0ae9, 0x25bc }, // [o] filledtribulletdown
{ 0x0aea, 0x261c }, // [o] leftpointer
{ 0x0aeb, 0x261e }, // [o] rightpointer
{ 0x0aec, 0x2663 }, // [.] club
{ 0x0aed, 0x2666 }, // [.] diamond
{ 0x0aee, 0x2665 }, // [.] heart
{ 0x0af0, 0x2720 }, // [.] maltesecross
{ 0x0af1, 0x2020 }, // [.] dagger
{ 0x0af2, 0x2021 }, // [.] doubledagger
{ 0x0af3, 0x2713 }, // [.] checkmark
{ 0x0af4, 0x2717 }, // [.] ballotcross
{ 0x0af5, 0x266f }, // [.] musicalsharp
{ 0x0af6, 0x266d }, // [.] musicalflat
{ 0x0af7, 0x2642 }, // [.] malesymbol
{ 0x0af8, 0x2640 }, // [.] femalesymbol
{ 0x0af9, 0x260e }, // [.] telephone
{ 0x0afa, 0x2315 }, // [.] telephonerecorder
{ 0x0afb, 0x2117 }, // [.] phonographcopyright
{ 0x0afc, 0x2038 }, // [.] caret
{ 0x0afd, 0x201a }, // [.] singlelowquotemark
{ 0x0afe, 0x201e }, // [.] doublelowquotemark
{ 0x0ba3, 0x003c }, // [d] leftcaret
{ 0x0ba6, 0x003e }, // [d] rightcaret
{ 0x0ba8, 0x2228 }, // [d] downcaret
{ 0x0ba9, 0x2227 }, // [d] upcaret
{ 0x0bc0, 0x00af }, // [d] overbar
{ 0x0bc2, 0x22a5 }, // [.] downtack
{ 0x0bc3, 0x2229 }, // [d] upshoe
{ 0x0bc4, 0x230a }, // [.] downstile
{ 0x0bc6, 0x005f }, // [d] underbar
{ 0x0bca, 0x2218 }, // [.] jot
{ 0x0bcc, 0x2395 }, // [.] quad
{ 0x0bce, 0x22a4 }, // [.] uptack
{ 0x0bcf, 0x25cb }, // [.] circle
{ 0x0bd3, 0x2308 }, // [.] upstile
{ 0x0bd6, 0x222a }, // [d] downshoe
{ 0x0bd8, 0x2283 }, // [d] rightshoe
{ 0x0bda, 0x2282 }, // [d] leftshoe
{ 0x0bdc, 0x22a2 }, // [.] lefttack
{ 0x0bfc, 0x22a3 }, // [.] righttack
{ 0x0cdf, 0x2017 }, // [.] hebrew_doublelowline
{ 0x0ce0, 0x05d0 }, // [.] hebrew_aleph
{ 0x0ce1, 0x05d1 }, // [.] hebrew_bet
{ 0x0ce1, 0x05d1 }, // [.] hebrew_beth
{ 0x0ce2, 0x05d2 }, // [.] hebrew_gimel
{ 0x0ce2, 0x05d2 }, // [.] hebrew_gimmel
{ 0x0ce3, 0x05d3 }, // [.] hebrew_dalet
{ 0x0ce3, 0x05d3 }, // [.] hebrew_daleth
{ 0x0ce4, 0x05d4 }, // [.] hebrew_he
{ 0x0ce5, 0x05d5 }, // [.] hebrew_waw
{ 0x0ce6, 0x05d6 }, // [.] hebrew_zain
{ 0x0ce6, 0x05d6 }, // [.] hebrew_zayin
{ 0x0ce7, 0x05d7 }, // [.] hebrew_chet
{ 0x0ce7, 0x05d7 }, // [.] hebrew_het
{ 0x0ce8, 0x05d8 }, // [.] hebrew_tet
{ 0x0ce8, 0x05d8 }, // [.] hebrew_teth
{ 0x0ce9, 0x05d9 }, // [.] hebrew_yod
{ 0x0cea, 0x05da }, // [.] hebrew_finalkaph
{ 0x0ceb, 0x05db }, // [.] hebrew_kaph
{ 0x0cec, 0x05dc }, // [.] hebrew_lamed
{ 0x0ced, 0x05dd }, // [.] hebrew_finalmem
{ 0x0cee, 0x05de }, // [.] hebrew_mem
{ 0x0cef, 0x05df }, // [.] hebrew_finalnun
{ 0x0cf0, 0x05e0 }, // [.] hebrew_nun
{ 0x0cf1, 0x05e1 }, // [.] hebrew_samech
{ 0x0cf1, 0x05e1 }, // [.] hebrew_samekh
{ 0x0cf2, 0x05e2 }, // [.] hebrew_ayin
{ 0x0cf3, 0x05e3 }, // [.] hebrew_finalpe
{ 0x0cf4, 0x05e4 }, // [.] hebrew_pe
{ 0x0cf5, 0x05e5 }, // [.] hebrew_finalzade
{ 0x0cf5, 0x05e5 }, // [.] hebrew_finalzadi
{ 0x0cf6, 0x05e6 }, // [.] hebrew_zade
{ 0x0cf6, 0x05e6 }, // [.] hebrew_zadi
{ 0x0cf7, 0x05e7 }, // [.] hebrew_kuf
{ 0x0cf7, 0x05e7 }, // [.] hebrew_qoph
{ 0x0cf8, 0x05e8 }, // [.] hebrew_resh
{ 0x0cf9, 0x05e9 }, // [.] hebrew_shin
{ 0x0cfa, 0x05ea }, // [.] hebrew_taf
{ 0x0cfa, 0x05ea }, // [.] hebrew_taw
{ 0x0da1, 0x0e01 }, // [.] Thai_kokai
{ 0x0da2, 0x0e02 }, // [.] Thai_khokhai
{ 0x0da3, 0x0e03 }, // [.] Thai_khokhuat
{ 0x0da4, 0x0e04 }, // [.] Thai_khokhwai
{ 0x0da5, 0x0e05 }, // [.] Thai_khokhon
{ 0x0da6, 0x0e06 }, // [.] Thai_khorakhang
{ 0x0da7, 0x0e07 }, // [.] Thai_ngongu
{ 0x0da8, 0x0e08 }, // [.] Thai_chochan
{ 0x0da9, 0x0e09 }, // [.] Thai_choching
{ 0x0daa, 0x0e0a }, // [.] Thai_chochang
{ 0x0dab, 0x0e0b }, // [.] Thai_soso
{ 0x0dac, 0x0e0c }, // [.] Thai_chochoe
{ 0x0dad, 0x0e0d }, // [.] Thai_yoying
{ 0x0dae, 0x0e0e }, // [.] Thai_dochada
{ 0x0daf, 0x0e0f }, // [.] Thai_topatak
{ 0x0db0, 0x0e10 }, // [.] Thai_thothan
{ 0x0db1, 0x0e11 }, // [.] Thai_thonangmontho
{ 0x0db2, 0x0e12 }, // [.] Thai_thophuthao
{ 0x0db3, 0x0e13 }, // [.] Thai_nonen
{ 0x0db4, 0x0e14 }, // [.] Thai_dodek
{ 0x0db5, 0x0e15 }, // [.] Thai_totao
{ 0x0db6, 0x0e16 }, // [.] Thai_thothung
{ 0x0db7, 0x0e17 }, // [.] Thai_thothahan
{ 0x0db8, 0x0e18 }, // [.] Thai_thothong
{ 0x0db9, 0x0e19 }, // [.] Thai_nonu
{ 0x0dba, 0x0e1a }, // [.] Thai_bobaimai
{ 0x0dbb, 0x0e1b }, // [.] Thai_popla
{ 0x0dbc, 0x0e1c }, // [.] Thai_phophung
{ 0x0dbd, 0x0e1d }, // [.] Thai_fofa
{ 0x0dbe, 0x0e1e }, // [.] Thai_phophan
{ 0x0dbf, 0x0e1f }, // [.] Thai_fofan
{ 0x0dc0, 0x0e20 }, // [.] Thai_phosamphao
{ 0x0dc1, 0x0e21 }, // [.] Thai_moma
{ 0x0dc2, 0x0e22 }, // [.] Thai_yoyak
{ 0x0dc3, 0x0e23 }, // [.] Thai_rorua
{ 0x0dc4, 0x0e24 }, // [.] Thai_ru
{ 0x0dc5, 0x0e25 }, // [.] Thai_loling
{ 0x0dc6, 0x0e26 }, // [.] Thai_lu
{ 0x0dc7, 0x0e27 }, // [.] Thai_wowaen
{ 0x0dc8, 0x0e28 }, // [.] Thai_sosala
{ 0x0dc9, 0x0e29 }, // [.] Thai_sorusi
{ 0x0dca, 0x0e2a }, // [.] Thai_sosua
{ 0x0dcb, 0x0e2b }, // [.] Thai_hohip
{ 0x0dcc, 0x0e2c }, // [.] Thai_lochula
{ 0x0dcd, 0x0e2d }, // [.] Thai_oang
{ 0x0dce, 0x0e2e }, // [.] Thai_honokhuk
{ 0x0dcf, 0x0e2f }, // [.] Thai_paiyannoi
{ 0x0dd0, 0x0e30 }, // [.] Thai_saraa
{ 0x0dd1, 0x0e31 }, // [.] Thai_maihanakat
{ 0x0dd2, 0x0e32 }, // [.] Thai_saraaa
{ 0x0dd3, 0x0e33 }, // [.] Thai_saraam
{ 0x0dd4, 0x0e34 }, // [.] Thai_sarai
{ 0x0dd5, 0x0e35 }, // [.] Thai_saraii
{ 0x0dd6, 0x0e36 }, // [.] Thai_saraue
{ 0x0dd7, 0x0e37 }, // [.] Thai_sarauee
{ 0x0dd8, 0x0e38 }, // [.] Thai_sarau
{ 0x0dd9, 0x0e39 }, // [.] Thai_sarauu
{ 0x0dda, 0x0e3a }, // [.] Thai_phinthu
{ 0x0ddf, 0x0e3f }, // [.] Thai_baht
{ 0x0de0, 0x0e40 }, // [.] Thai_sarae
{ 0x0de1, 0x0e41 }, // [.] Thai_saraae
{ 0x0de2, 0x0e42 }, // [.] Thai_sarao
{ 0x0de3, 0x0e43 }, // [.] Thai_saraaimaimuan
{ 0x0de4, 0x0e44 }, // [.] Thai_saraaimaimalai
{ 0x0de5, 0x0e45 }, // [.] Thai_lakkhangyao
{ 0x0de6, 0x0e46 }, // [.] Thai_maiyamok
{ 0x0de7, 0x0e47 }, // [.] Thai_maitaikhu
{ 0x0de8, 0x0e48 }, // [.] Thai_maiek
{ 0x0de9, 0x0e49 }, // [.] Thai_maitho
{ 0x0dea, 0x0e4a }, // [.] Thai_maitri
{ 0x0deb, 0x0e4b }, // [.] Thai_maichattawa
{ 0x0dec, 0x0e4c }, // [.] Thai_thanthakhat
{ 0x0ded, 0x0e4d }, // [.] Thai_nikhahit
{ 0x0df0, 0x0e50 }, // [.] Thai_leksun
{ 0x0df1, 0x0e51 }, // [.] Thai_leknung
{ 0x0df2, 0x0e52 }, // [.] Thai_leksong
{ 0x0df3, 0x0e53 }, // [.] Thai_leksam
{ 0x0df4, 0x0e54 }, // [.] Thai_leksi
{ 0x0df5, 0x0e55 }, // [.] Thai_lekha
{ 0x0df6, 0x0e56 }, // [.] Thai_lekhok
{ 0x0df7, 0x0e57 }, // [.] Thai_lekchet
{ 0x0df8, 0x0e58 }, // [.] Thai_lekpaet
{ 0x0df9, 0x0e59 }, // [.] Thai_lekkao
{ 0x0ea1, 0x3131 }, // [f] Hangul_Kiyeog
{ 0x0ea2, 0x3132 }, // [f] Hangul_SsangKiyeog
{ 0x0ea3, 0x3133 }, // [f] Hangul_KiyeogSios
{ 0x0ea4, 0x3134 }, // [f] Hangul_Nieun
{ 0x0ea5, 0x3135 }, // [f] Hangul_NieunJieuj
{ 0x0ea6, 0x3136 }, // [f] Hangul_NieunHieuh
{ 0x0ea7, 0x3137 }, // [f] Hangul_Dikeud
{ 0x0ea8, 0x3138 }, // [f] Hangul_SsangDikeud
{ 0x0ea9, 0x3139 }, // [f] Hangul_Rieul
{ 0x0eaa, 0x313a }, // [f] Hangul_RieulKiyeog
{ 0x0eab, 0x313b }, // [f] Hangul_RieulMieum
{ 0x0eac, 0x313c }, // [f] Hangul_RieulPieub
{ 0x0ead, 0x313d }, // [f] Hangul_RieulSios
{ 0x0eae, 0x313e }, // [f] Hangul_RieulTieut
{ 0x0eaf, 0x313f }, // [f] Hangul_RieulPhieuf
{ 0x0eb0, 0x3140 }, // [f] Hangul_RieulHieuh
{ 0x0eb1, 0x3141 }, // [f] Hangul_Mieum
{ 0x0eb2, 0x3142 }, // [f] Hangul_Pieub
{ 0x0eb3, 0x3143 }, // [f] Hangul_SsangPieub
{ 0x0eb4, 0x3144 }, // [f] Hangul_PieubSios
{ 0x0eb5, 0x3145 }, // [f] Hangul_Sios
{ 0x0eb6, 0x3146 }, // [f] Hangul_SsangSios
{ 0x0eb7, 0x3147 }, // [f] Hangul_Ieung
{ 0x0eb8, 0x3148 }, // [f] Hangul_Jieuj
{ 0x0eb9, 0x3149 }, // [f] Hangul_SsangJieuj
{ 0x0eba, 0x314a }, // [f] Hangul_Cieuc
{ 0x0ebb, 0x314b }, // [f] Hangul_Khieuq
{ 0x0ebc, 0x314c }, // [f] Hangul_Tieut
{ 0x0ebd, 0x314d }, // [f] Hangul_Phieuf
{ 0x0ebe, 0x314e }, // [f] Hangul_Hieuh
{ 0x0ebf, 0x314f }, // [f] Hangul_A
{ 0x0ec0, 0x3150 }, // [f] Hangul_AE
{ 0x0ec1, 0x3151 }, // [f] Hangul_YA
{ 0x0ec2, 0x3152 }, // [f] Hangul_YAE
{ 0x0ec3, 0x3153 }, // [f] Hangul_EO
{ 0x0ec4, 0x3154 }, // [f] Hangul_E
{ 0x0ec5, 0x3155 }, // [f] Hangul_YEO
{ 0x0ec6, 0x3156 }, // [f] Hangul_YE
{ 0x0ec7, 0x3157 }, // [f] Hangul_O
{ 0x0ec8, 0x3158 }, // [f] Hangul_WA
{ 0x0ec9, 0x3159 }, // [f] Hangul_WAE
{ 0x0eca, 0x315a }, // [f] Hangul_OE
{ 0x0ecb, 0x315b }, // [f] Hangul_YO
{ 0x0ecc, 0x315c }, // [f] Hangul_U
{ 0x0ecd, 0x315d }, // [f] Hangul_WEO
{ 0x0ece, 0x315e }, // [f] Hangul_WE
{ 0x0ecf, 0x315f }, // [f] Hangul_WI
{ 0x0ed0, 0x3160 }, // [f] Hangul_YU
{ 0x0ed1, 0x3161 }, // [f] Hangul_EU
{ 0x0ed2, 0x3162 }, // [f] Hangul_YI
{ 0x0ed3, 0x3163 }, // [f] Hangul_I
{ 0x0ed4, 0x11a8 }, // [f] Hangul_J_Kiyeog
{ 0x0ed5, 0x11a9 }, // [f] Hangul_J_SsangKiyeog
{ 0x0ed6, 0x11aa }, // [f] Hangul_J_KiyeogSios
{ 0x0ed7, 0x11ab }, // [f] Hangul_J_Nieun
{ 0x0ed8, 0x11ac }, // [f] Hangul_J_NieunJieuj
{ 0x0ed9, 0x11ad }, // [f] Hangul_J_NieunHieuh
{ 0x0eda, 0x11ae }, // [f] Hangul_J_Dikeud
{ 0x0edb, 0x11af }, // [f] Hangul_J_Rieul
{ 0x0edc, 0x11b0 }, // [f] Hangul_J_RieulKiyeog
{ 0x0edd, 0x11b1 }, // [f] Hangul_J_RieulMieum
{ 0x0ede, 0x11b2 }, // [f] Hangul_J_RieulPieub
{ 0x0edf, 0x11b3 }, // [f] Hangul_J_RieulSios
{ 0x0ee0, 0x11b4 }, // [f] Hangul_J_RieulTieut
{ 0x0ee1, 0x11b5 }, // [f] Hangul_J_RieulPhieuf
{ 0x0ee2, 0x11b6 }, // [f] Hangul_J_RieulHieuh
{ 0x0ee3, 0x11b7 }, // [f] Hangul_J_Mieum
{ 0x0ee4, 0x11b8 }, // [f] Hangul_J_Pieub
{ 0x0ee5, 0x11b9 }, // [f] Hangul_J_PieubSios
{ 0x0ee6, 0x11ba }, // [f] Hangul_J_Sios
{ 0x0ee7, 0x11bb }, // [f] Hangul_J_SsangSios
{ 0x0ee8, 0x11bc }, // [f] Hangul_J_Ieung
{ 0x0ee9, 0x11bd }, // [f] Hangul_J_Jieuj
{ 0x0eea, 0x11be }, // [f] Hangul_J_Cieuc
{ 0x0eeb, 0x11bf }, // [f] Hangul_J_Khieuq
{ 0x0eec, 0x11c0 }, // [f] Hangul_J_Tieut
{ 0x0eed, 0x11c1 }, // [f] Hangul_J_Phieuf
{ 0x0eee, 0x11c2 }, // [f] Hangul_J_Hieuh
{ 0x0eef, 0x316d }, // [f] Hangul_RieulYeorinHieuh
{ 0x0ef0, 0x3171 }, // [f] Hangul_SunkyeongeumMieum
{ 0x0ef1, 0x3178 }, // [f] Hangul_SunkyeongeumPieub
{ 0x0ef2, 0x317f }, // [f] Hangul_PanSios
{ 0x0ef3, 0x3181 }, // [f] Hangul_KkogjiDalrinIeung
{ 0x0ef4, 0x3184 }, // [f] Hangul_SunkyeongeumPhieuf
{ 0x0ef5, 0x3186 }, // [f] Hangul_YeorinHieuh
{ 0x0ef6, 0x318d }, // [f] Hangul_AraeA
{ 0x0ef7, 0x318e }, // [f] Hangul_AraeAE
{ 0x0ef8, 0x11eb }, // [f] Hangul_J_PanSios
{ 0x0ef9, 0x11f0 }, // [f] Hangul_J_KkogjiDalrinIeung
{ 0x0efa, 0x11f9 }, // [f] Hangul_J_YeorinHieuh
{ 0x0eff, 0x20a9 }, // [o] Korean_Won
{ 0x12a1, 0x1e02 }, // [u] Babovedot
{ 0x12a2, 0x1e03 }, // [u] babovedot
{ 0x12a6, 0x1e0a }, // [u] Dabovedot
{ 0x12a8, 0x1e80 }, // [u] Wgrave
{ 0x12aa, 0x1e82 }, // [u] Wacute
{ 0x12ab, 0x1e0b }, // [u] dabovedot
{ 0x12ac, 0x1ef2 }, // [u] Ygrave
{ 0x12b0, 0x1e1e }, // [u] Fabovedot
{ 0x12b1, 0x1e1f }, // [u] fabovedot
{ 0x12b4, 0x1e40 }, // [u] Mabovedot
{ 0x12b5, 0x1e41 }, // [u] mabovedot
{ 0x12b7, 0x1e56 }, // [u] Pabovedot
{ 0x12b8, 0x1e81 }, // [u] wgrave
{ 0x12b9, 0x1e57 }, // [u] pabovedot
{ 0x12ba, 0x1e83 }, // [u] wacute
{ 0x12bb, 0x1e60 }, // [u] Sabovedot
{ 0x12bc, 0x1ef3 }, // [u] ygrave
{ 0x12bd, 0x1e84 }, // [u] Wdiaeresis
{ 0x12be, 0x1e85 }, // [u] wdiaeresis
{ 0x12bf, 0x1e61 }, // [u] sabovedot
{ 0x12d0, 0x0174 }, // [u] Wcircumflex
{ 0x12d7, 0x1e6a }, // [u] Tabovedot
{ 0x12de, 0x0176 }, // [u] Ycircumflex
{ 0x12f0, 0x0175 }, // [u] wcircumflex
{ 0x12f7, 0x1e6b }, // [u] tabovedot
{ 0x12fe, 0x0177 }, // [u] ycircumflex
{ 0x13bc, 0x0152 }, // [.] OE
{ 0x13bd, 0x0153 }, // [.] oe
{ 0x13be, 0x0178 }, // [.] Ydiaeresis
{ 0x14a2, 0x0587 }, // [u] Armenian_ligature_ew
{ 0x14a3, 0x0589 }, // [u] Armenian_verjaket
{ 0x14a4, 0x0029 }, // [r] Armenian_parenright
{ 0x14a5, 0x0028 }, // [r] Armenian_parenleft
{ 0x14a6, 0x00bb }, // [r] Armenian_guillemotright
{ 0x14a7, 0x00ab }, // [r] Armenian_guillemotleft
{ 0x14a8, 0x2014 }, // [r] Armenian_em_dash
{ 0x14a9, 0x002e }, // [r] Armenian_mijaket
{ 0x14aa, 0x055d }, // [u] Armenian_but
{ 0x14ab, 0x002c }, // [r] Armenian_comma
{ 0x14ac, 0x2013 }, // [r] Armenian_en_dash
{ 0x14ad, 0x058a }, // [u] Armenian_yentamna
{ 0x14ae, 0x2026 }, // [r] Armenian_ellipsis
{ 0x14af, 0x055c }, // [u] Armenian_amanak
{ 0x14b0, 0x055b }, // [u] Armenian_shesht
{ 0x14b1, 0x055e }, // [u] Armenian_paruyk
{ 0x14b2, 0x0531 }, // [u] Armenian_AYB
{ 0x14b3, 0x0561 }, // [u] Armenian_ayb
{ 0x14b4, 0x0532 }, // [u] Armenian_BEN
{ 0x14b5, 0x0562 }, // [u] Armenian_ben
{ 0x14b6, 0x0533 }, // [u] Armenian_GIM
{ 0x14b7, 0x0563 }, // [u] Armenian_gim
{ 0x14b8, 0x0534 }, // [u] Armenian_DA
{ 0x14b9, 0x0564 }, // [u] Armenian_da
{ 0x14ba, 0x0535 }, // [u] Armenian_YECH
{ 0x14bb, 0x0565 }, // [u] Armenian_yech
{ 0x14bc, 0x0536 }, // [u] Armenian_ZA
{ 0x14bd, 0x0566 }, // [u] Armenian_za
{ 0x14be, 0x0537 }, // [u] Armenian_E
{ 0x14bf, 0x0567 }, // [u] Armenian_e
{ 0x14c0, 0x0538 }, // [u] Armenian_AT
{ 0x14c1, 0x0568 }, // [u] Armenian_at
{ 0x14c2, 0x0539 }, // [u] Armenian_TO
{ 0x14c3, 0x0569 }, // [u] Armenian_to
{ 0x14c4, 0x053a }, // [u] Armenian_ZHE
{ 0x14c5, 0x056a }, // [u] Armenian_zhe
{ 0x14c6, 0x053b }, // [u] Armenian_INI
{ 0x14c7, 0x056b }, // [u] Armenian_ini
{ 0x14c8, 0x053c }, // [u] Armenian_LYUN
{ 0x14c9, 0x056c }, // [u] Armenian_lyun
{ 0x14ca, 0x053d }, // [u] Armenian_KHE
{ 0x14cb, 0x056d }, // [u] Armenian_khe
{ 0x14cc, 0x053e }, // [u] Armenian_TSA
{ 0x14cd, 0x056e }, // [u] Armenian_tsa
{ 0x14ce, 0x053f }, // [u] Armenian_KEN
{ 0x14cf, 0x056f }, // [u] Armenian_ken
{ 0x14d0, 0x0540 }, // [u] Armenian_HO
{ 0x14d1, 0x0570 }, // [u] Armenian_ho
{ 0x14d2, 0x0541 }, // [u] Armenian_DZA
{ 0x14d3, 0x0571 }, // [u] Armenian_dza
{ 0x14d4, 0x0542 }, // [u] Armenian_GHAT
{ 0x14d5, 0x0572 }, // [u] Armenian_ghat
{ 0x14d6, 0x0543 }, // [u] Armenian_TCHE
{ 0x14d7, 0x0573 }, // [u] Armenian_tche
{ 0x14d8, 0x0544 }, // [u] Armenian_MEN
{ 0x14d9, 0x0574 }, // [u] Armenian_men
{ 0x14da, 0x0545 }, // [u] Armenian_HI
{ 0x14db, 0x0575 }, // [u] Armenian_hi
{ 0x14dc, 0x0546 }, // [u] Armenian_NU
{ 0x14dd, 0x0576 }, // [u] Armenian_nu
{ 0x14de, 0x0547 }, // [u] Armenian_SHA
{ 0x14df, 0x0577 }, // [u] Armenian_sha
{ 0x14e0, 0x0548 }, // [u] Armenian_VO
{ 0x14e1, 0x0578 }, // [u] Armenian_vo
{ 0x14e2, 0x0549 }, // [u] Armenian_CHA
{ 0x14e3, 0x0579 }, // [u] Armenian_cha
{ 0x14e4, 0x054a }, // [u] Armenian_PE
{ 0x14e5, 0x057a }, // [u] Armenian_pe
{ 0x14e6, 0x054b }, // [u] Armenian_JE
{ 0x14e7, 0x057b }, // [u] Armenian_je
{ 0x14e8, 0x054c }, // [u] Armenian_RA
{ 0x14e9, 0x057c }, // [u] Armenian_ra
{ 0x14ea, 0x054d }, // [u] Armenian_SE
{ 0x14eb, 0x057d }, // [u] Armenian_se
{ 0x14ec, 0x054e }, // [u] Armenian_VEV
{ 0x14ed, 0x057e }, // [u] Armenian_vev
{ 0x14ee, 0x054f }, // [u] Armenian_TYUN
{ 0x14ef, 0x057f }, // [u] Armenian_tyun
{ 0x14f0, 0x0550 }, // [u] Armenian_RE
{ 0x14f1, 0x0580 }, // [u] Armenian_re
{ 0x14f2, 0x0551 }, // [u] Armenian_TSO
{ 0x14f3, 0x0581 }, // [u] Armenian_tso
{ 0x14f4, 0x0552 }, // [u] Armenian_VYUN
{ 0x14f5, 0x0582 }, // [u] Armenian_vyun
{ 0x14f6, 0x0553 }, // [u] Armenian_PYUR
{ 0x14f7, 0x0583 }, // [u] Armenian_pyur
{ 0x14f8, 0x0554 }, // [u] Armenian_KE
{ 0x14f9, 0x0584 }, // [u] Armenian_ke
{ 0x14fa, 0x0555 }, // [u] Armenian_O
{ 0x14fb, 0x0585 }, // [u] Armenian_o
{ 0x14fc, 0x0556 }, // [u] Armenian_FE
{ 0x14fd, 0x0586 }, // [u] Armenian_fe
{ 0x14fe, 0x055a }, // [u] Armenian_apostrophe
{ 0x14ff, 0x00a7 }, // [r] Armenian_section_sign
{ 0x15d0, 0x10d0 }, // [u] Georgian_an
{ 0x15d1, 0x10d1 }, // [u] Georgian_ban
{ 0x15d2, 0x10d2 }, // [u] Georgian_gan
{ 0x15d3, 0x10d3 }, // [u] Georgian_don
{ 0x15d4, 0x10d4 }, // [u] Georgian_en
{ 0x15d5, 0x10d5 }, // [u] Georgian_vin
{ 0x15d6, 0x10d6 }, // [u] Georgian_zen
{ 0x15d7, 0x10d7 }, // [u] Georgian_tan
{ 0x15d8, 0x10d8 }, // [u] Georgian_in
{ 0x15d9, 0x10d9 }, // [u] Georgian_kan
{ 0x15da, 0x10da }, // [u] Georgian_las
{ 0x15db, 0x10db }, // [u] Georgian_man
{ 0x15dc, 0x10dc }, // [u] Georgian_nar
{ 0x15dd, 0x10dd }, // [u] Georgian_on
{ 0x15de, 0x10de }, // [u] Georgian_par
{ 0x15df, 0x10df }, // [u] Georgian_zhar
{ 0x15e0, 0x10e0 }, // [u] Georgian_rae
{ 0x15e1, 0x10e1 }, // [u] Georgian_san
{ 0x15e2, 0x10e2 }, // [u] Georgian_tar
{ 0x15e3, 0x10e3 }, // [u] Georgian_un
{ 0x15e4, 0x10e4 }, // [u] Georgian_phar
{ 0x15e5, 0x10e5 }, // [u] Georgian_khar
{ 0x15e6, 0x10e6 }, // [u] Georgian_ghan
{ 0x15e7, 0x10e7 }, // [u] Georgian_qar
{ 0x15e8, 0x10e8 }, // [u] Georgian_shin
{ 0x15e9, 0x10e9 }, // [u] Georgian_chin
{ 0x15ea, 0x10ea }, // [u] Georgian_can
{ 0x15eb, 0x10eb }, // [u] Georgian_jil
{ 0x15ec, 0x10ec }, // [u] Georgian_cil
{ 0x15ed, 0x10ed }, // [u] Georgian_char
{ 0x15ee, 0x10ee }, // [u] Georgian_xan
{ 0x15ef, 0x10ef }, // [u] Georgian_jhan
{ 0x15f0, 0x10f0 }, // [u] Georgian_hae
{ 0x15f1, 0x10f1 }, // [u] Georgian_he
{ 0x15f2, 0x10f2 }, // [u] Georgian_hie
{ 0x15f3, 0x10f3 }, // [u] Georgian_we
{ 0x15f4, 0x10f4 }, // [u] Georgian_har
{ 0x15f5, 0x10f5 }, // [u] Georgian_hoe
{ 0x15f6, 0x10f6 }, // [u] Georgian_fi
{ 0x16a3, 0x1e8a }, // [u] Xabovedot
{ 0x16a6, 0x012c }, // [u] Ibreve
{ 0x16a9, 0x01b5 }, // [u] Zstroke
{ 0x16aa, 0x01e6 }, // [u] Gcaron
{ 0x16af, 0x019f }, // [u] Obarred
{ 0x16b3, 0x1e8b }, // [u] xabovedot
{ 0x16b6, 0x012d }, // [u] ibreve
{ 0x16b9, 0x01b6 }, // [u] zstroke
{ 0x16ba, 0x01e7 }, // [u] gcaron
{ 0x16bd, 0x01d2 }, // [u] ocaron
{ 0x16bf, 0x0275 }, // [u] obarred
{ 0x16c6, 0x018f }, // [u] SCHWA
{ 0x16d1, 0x1e36 }, // [u] Lbelowdot
{ 0x16e1, 0x1e37 }, // [u] lbelowdot
{ 0x16f6, 0x0259 }, // [u] schwa
{ 0x1e9f, 0x0303 }, // [r] combining_tilde
{ 0x1ef2, 0x0300 }, // [r] combining_grave
{ 0x1ef3, 0x0301 }, // [r] combining_acute
{ 0x1efa, 0x01a0 }, // [u] Ohorn
{ 0x1efb, 0x01a1 }, // [u] ohorn
{ 0x1efc, 0x01af }, // [u] Uhorn
{ 0x1efd, 0x01b0 }, // [u] uhorn
{ 0x1efe, 0x0309 }, // [r] combining_hook
{ 0x1eff, 0x0323 }, // [r] combining_belowdot
{ 0xfe50, 0x0300 }, // [f] dead_grave
{ 0xfe51, 0x0301 }, // [f] dead_acute
{ 0xfe52, 0x0302 }, // [f] dead_circumflex
{ 0xfe53, 0x0303 }, // [f] dead_tilde
{ 0xfe54, 0x0304 }, // [f] dead_macron
{ 0xfe55, 0x0306 }, // [f] dead_breve
{ 0xfe56, 0x0307 }, // [f] dead_abovedot
{ 0xfe57, 0x0308 }, // [f] dead_diaeresis
{ 0xfe58, 0x030a }, // [f] dead_abovering
{ 0xfe59, 0x030b }, // [f] dead_doubleacute
{ 0xfe5a, 0x030c }, // [f] dead_caron
{ 0xfe5b, 0x0327 }, // [f] dead_cedilla
{ 0xfe5c, 0x0328 }, // [f] dead_ogonek
{ 0xfe5d, 0x0345 }, // [f] dead_iota
{ 0xfe5e, 0x3099 }, // [f] dead_voiced_sound
{ 0xfe5f, 0x309a }, // [f] dead_semivoiced_sound
{ 0xfe60, 0x0323 }, // [f] dead_belowdot
{ 0xfe61, 0x0309 }, // [f] dead_hook
{ 0xfe62, 0x031b }, // [f] dead_horn
{ 0xfe62, 0x031b }, // [f] dead_horn
{ 0xff08, 0x0008 }, // [f] BackSpace
{ 0xff09, 0x0009 }, // [f] Tab
{ 0xff0a, 0x000a }, // [f] Linefeed
{ 0xff0b, 0x000b }, // [f] Clear
{ 0xff0d, 0x000d }, // [f] Return
{ 0xff13, 0x0013 }, // [f] Pause
{ 0xff14, 0x0014 }, // [f] Scroll_Lock
{ 0xff15, 0x0015 }, // [f] Sys_Req
{ 0xff1b, 0x001b }, // [f] Escape
{ 0xff80, 0x0020 }, // [f] KP_Space
{ 0xff89, 0x0009 }, // [f] KP_Tab
{ 0xff8d, 0x000d }, // [f] KP_Enter
{ 0xffaa, 0x002a }, // [f] KP_Multiply
{ 0xffab, 0x002b }, // [f] KP_Add
{ 0xffac, 0x002c }, // [f] KP_Separator
{ 0xffad, 0x002d }, // [f] KP_Subtract
{ 0xffae, 0x002e }, // [f] KP_Decimal
{ 0xffaf, 0x002f }, // [f] KP_Divide
{ 0xffb0, 0x0030 }, // [f] KP_0
{ 0xffb1, 0x0031 }, // [f] KP_1
{ 0xffb2, 0x0032 }, // [f] KP_2
{ 0xffb3, 0x0033 }, // [f] KP_3
{ 0xffb4, 0x0034 }, // [f] KP_4
{ 0xffb5, 0x0035 }, // [f] KP_5
{ 0xffb6, 0x0036 }, // [f] KP_6
{ 0xffb7, 0x0037 }, // [f] KP_7
{ 0xffb8, 0x0038 }, // [f] KP_8
{ 0xffb9, 0x0039 }, // [f] KP_9
{ 0xffbd, 0x003d }  // [f] KP_Equal
};



void osiInput::_keysym2unicode(KeySym *ks, uint32 *ret) {
  int min= 0;
  int max= sizeof(ks2unicode)/ sizeof(struct CodePair)- 1;
  int mid;
  
  /// check for Latin-1 characters (1:1 mapping)
  if( ((*ks)>= 0x0020 && (*ks)<= 0x007e) ||
      ((*ks)>= 0x00a0 && (*ks)<= 0x00ff) ) {
    *ret= *ks;
    return;
  }

  /// check for directly encoded unicode characters
  if (((*ks)& 0xff000000) == 0x01000000) {
    *ret= (*ks)& 0x00ffffff;
    return;
  }
  
  /// binary search in table */
  while (max >= min) {
    mid = (min+ max)/ 2;
    if (ks2unicode[mid].keysym < *ks)
	    min= mid+ 1;
    else if (ks2unicode[mid].keysym > *ks)
	    max= mid- 1;
    else {                                // found it
      *ret= ks2unicode[mid].unicode;
	    return;
    }
  }

  /// just asume the keysym is 1:1 mapped (THIS PART IS DIFFERENT FROM THE ORIGINAL FILE)
  /// must test the resulting unicode if is valid or not, but not in this func
  *ret= *ks;
}




void osiInput::_getUnicode(KeySym *ks, uint32 *ret) {
  
  // only unicode printable characters are returned
  // this func can return 0! STRING TERMINATOR CONFUSION warning!!!!

  
  // these checks are hand-picked from keysymdef.h.
  /// there are printable characters in between these (maybe some checks can be combined, but...)
  if((*ks>= 0xff08) && (*ks <= 0xff7f))  { *ret= 0; return; }
  if((*ks>= 0xff89) && (*ks <= 0xff9f))  { *ret= 0; return; }
  if((*ks>= 0xffbe) && (*ks <= 0xffee))  { *ret= 0; return; }
  if((*ks>= 0xfe01) && (*ks <= 0xfe13))  { *ret= 0; return; }
  if((*ks>= 0xfe20) && (*ks <= 0xfe34))  { *ret= 0; return; }
  if((*ks>= 0xfed0) && (*ks <= 0xfed5))  { *ret= 0; return; }
  if((*ks>= 0xfe70) && (*ks <= 0xfe7a))  { *ret= 0; return; }
  if((*ks>= 0xfee0) && (*ks <= 0xfefc))  { *ret= 0; return; }
  if(*ks== 0xffff)                       { *ret= 0; return; } // Delete, rubout
  
  /// call the main transform func
  _keysym2unicode(ks, ret);
  
  // further checks of resulting unicode value must be placed from here  

  /// check for the limit of Unicode <--------------------- (maybe this changes in the future)
  if(*ret>= 0x10FFFF)
    *ret= 0xFFFD;
  
  /// it can't be a utf-16 surrogate
  if( (*ret>= 0xD800) && (*ret<= 0xDFFF) )  
    *ret= 0xFFFD;
}

#endif /// OS_LINUX




// is this function more tied to OSchar.cpp? should this be in osinput.cpp, as it is part of Keyboard?

void osiKeyboard::_doManip() {
  // only string manipulator characters/keys are returned

  // XK_Tab 0xff09 NOT RETURNED ATM, SUBJECT TO CHANGE?

  /// check if shift is pressed; if it is, check for selection changes chars
  if(key[in.Kv.lshift] || key[in.Kv.rshift]) {
    if(key[in.Kv.home])   { _addManip(Kch_selHome, &osi.eventTime);   return; }
    if(key[in.Kv.end])    { _addManip(Kch_selEnd, &osi.eventTime);    return; }
    if(key[in.Kv.pgup])   { _addManip(Kch_selPgUp, &osi.eventTime);   return; }
    if(key[in.Kv.pgdown]) { _addManip(Kch_selPgDown, &osi.eventTime); return; }
    if(key[in.Kv.left])   { _addManip(Kch_selLeft, &osi.eventTime);   return; }
    if(key[in.Kv.up])     { _addManip(Kch_selUp, &osi.eventTime);     return; }
    if(key[in.Kv.right])  { _addManip(Kch_selRight, &osi.eventTime);  return; }
    if(key[in.Kv.down])   { _addManip(Kch_selDown, &osi.eventTime);   return; }
    
    if(!numLock) {
      if(key[in.Kv.kp7]) { _addManip(Kch_selHome, &osi.eventTime);   return; }
      if(key[in.Kv.kp1]) { _addManip(Kch_selEnd, &osi.eventTime);    return; }
      if(key[in.Kv.kp9]) { _addManip(Kch_selPgUp, &osi.eventTime);   return; }
      if(key[in.Kv.kp3]) { _addManip(Kch_selPgDown, &osi.eventTime); return; }
      if(key[in.Kv.kp4]) { _addManip(Kch_selLeft, &osi.eventTime);   return; }
      if(key[in.Kv.kp8]) { _addManip(Kch_selUp, &osi.eventTime);     return; }
      if(key[in.Kv.kp6]) { _addManip(Kch_selRight, &osi.eventTime);  return; }
      if(key[in.Kv.kp2]) { _addManip(Kch_selDown, &osi.eventTime);   return; }
    }
  } /// if shift key is pressed
  
  if(key[in.Kv.enter])     { _addManip(Kch_enter, &osi.eventTime);     return; }
  if(key[in.Kv.kpenter])   { _addManip(Kch_enter, &osi.eventTime);     return; }

  if(key[in.Kv.backspace]) { _addManip(Kch_backSpace, &osi.eventTime); return; }
  if(key[in.Kv.del])       { _addManip(Kch_delete, &osi.eventTime);    return; }
  if(key[in.Kv.kpdel])     { _addManip(Kch_delete, &osi.eventTime);    return; }
  
  if(key[in.Kv.home])      { _addManip(Kch_home, &osi.eventTime);      return; }
  if(key[in.Kv.end])       { _addManip(Kch_end, &osi.eventTime);       return; }
  if(key[in.Kv.pgup])      { _addManip(Kch_pgUp, &osi.eventTime);      return; }
  if(key[in.Kv.pgdown])    { _addManip(Kch_pgDown, &osi.eventTime);    return; }
    
  if(key[in.Kv.left])      { _addManip(Kch_left, &osi.eventTime);      return; }
  if(key[in.Kv.up])        { _addManip(Kch_up, &osi.eventTime);        return; }
  if(key[in.Kv.right])     { _addManip(Kch_right, &osi.eventTime);     return; }
  if(key[in.Kv.down])      { _addManip(Kch_down, &osi.eventTime);      return; }
         
  if(!numLock) {         
    if(key[in.Kv.kp7])  { _addManip(Kch_home, &osi.eventTime);      return; }
    if(key[in.Kv.kp1])  { _addManip(Kch_end, &osi.eventTime);       return; }
    if(key[in.Kv.kp9])  { _addManip(Kch_pgUp, &osi.eventTime);      return; }
    if(key[in.Kv.kp3])  { _addManip(Kch_pgDown, &osi.eventTime);    return; }
    if(key[in.Kv.kp4])  { _addManip(Kch_left, &osi.eventTime);      return; }
    if(key[in.Kv.kp8])  { _addManip(Kch_up, &osi.eventTime);        return; }
    if(key[in.Kv.kp6])  { _addManip(Kch_right, &osi.eventTime);     return; }
    if(key[in.Kv.kp2])  { _addManip(Kch_down, &osi.eventTime);      return; }
  }


  // no clue what to do on a japanese keyboard, for example ... :\
  // must find out what their default 'copy/cut/paste' keys combs are
  // IT SEEMS TO BE THE SAME, SO NOTHING TO BOTHER ABOUT OTHER COUNTRYES KEYBOARDS
  
  /// 'copy' key combination checks
  if((key[in.Kv.lctrl]  || key[in.Kv.rctrl]) &&     /// ctrl +
     (key[in.Kv.insert] || key[in.Kv.kp0])) {       /// insert
    _addManip(Kch_copy, &osi.eventTime);
    return;
  }
  if((key[in.Kv.lctrl] || key[in.Kv.rctrl]) &&      /// ctrl +
      key[in.Kv.c]) {                               /// c
    _addManip(Kch_copy, &osi.eventTime);
    return;
  }
    
  /// 'cut' key combination checks
  if((key[in.Kv.lctrl] || key[in.Kv.rctrl])  &&     /// ctrl +
     (key[in.Kv.del]   || key[in.Kv.kpdel]))  {     /// del
    _addManip(Kch_cut, &osi.eventTime);
    return;
  }
  if((key[in.Kv.lctrl] || key[in.Kv.rctrl]) &&      /// ctrl +
      key[in.Kv.x]) {                               /// x
    _addManip(Kch_cut, &osi.eventTime);
    return;
  }
    
  /// 'paste' key combination checks
  if((key[in.Kv.lshift] || key[in.Kv.rshift]) &&    /// shift +
     (key[in.Kv.insert] || key[in.Kv.kp0])) {       /// insert
    _addManip(Kch_paste, &osi.eventTime);
    return;
  }
  if((key[in.Kv.lctrl] || key[in.Kv.rctrl]) &&      /// ctrl +
      key[in.Kv.v]) {                               /// v
    _addManip(Kch_paste, &osi.eventTime);
    return;
  }
  
}





// Keyboard key codes (standard keys only) for each OS

void _Kv::populate() {
  #ifdef OS_WIN

  
  if(in.k.mode== 2) {
    esc=        VK_ESCAPE;
    enter=      VK_RETURN;
    kpenter=    VK_RETURN;             // no difference
    tab=        VK_TAB;
    space=      VK_SPACE;
    backspace=  VK_BACK;
    insert=     VK_INSERT;
    del=        VK_DELETE;
    home=       VK_HOME;
    end=        VK_END;
    pgup=       0x21;
    pgdown=     0x22;
    rshift=     VK_RSHIFT;
    lshift=     VK_LSHIFT;
    rctrl=      VK_RCONTROL;
    lctrl=      VK_LCONTROL;
    ralt=       VK_RMENU;
    lalt=       VK_LMENU;
    f1=         VK_F1;
    f2=         VK_F2;
    f3=         VK_F3;
    f4=         VK_F4;
    f5=         VK_F5;
    f6=         VK_F6;
    f7=         VK_F7;
    f8=         VK_F8;
    f9=         VK_F9;
    f10=        VK_F10;
    f11=        VK_F11;
    f12=        VK_F12;
    left=       VK_LEFT;
    up=         VK_UP;
    right=      VK_RIGHT;
    down=       VK_DOWN;
    prtsc=      0x2C;
    pause=      VK_PAUSE;
    capslock=   VK_CAPITAL;
    scrolllock= VK_SCROLL;
    numlock=    VK_NUMLOCK;
    q=          0x51;
    w=          0x57;
    e=          0x45;
    r=          0x52;
    t=          0x54;
    y=          0x59;
    u=          0x55;
    i=          0x49;
    o=          0x4F;
    p=          0x50;
    a=          0x41;
    s=          0x53;
    d=          0x44;
    f=          0x46;
    g=          0x47;
    h=          0x48;
    j=          0x4A;
    k=          0x4B;
    l=          0x4C;
    z=          0x5A;
    x=          0x58;
    c=          0x43;
    v=          0x56;
    b=          0x42;
    n=          0x4E;
    m=          0x4D;
    n1=         0x31;
    n2=         0x32;
    n3=         0x33;
    n4=         0x34;
    n5=         0x35;
    n6=         0x36;
    n7=         0x37;
    n8=         0x38;
    n9=         0x39;
    n0=         0x30;
    kp1=        VK_NUMPAD1;
    kp2=        VK_NUMPAD2;
    kp3=        VK_NUMPAD3;
    kp4=        VK_NUMPAD4;
    kp5=        VK_NUMPAD5;
    kp6=        VK_NUMPAD6;
    kp7=        VK_NUMPAD7;
    kp8=        VK_NUMPAD8;
    kp9=        VK_NUMPAD9;
    kp0=        VK_NUMPAD0;
    minus=      VK_SUBTRACT;
    equal=      0xBB;
    backslash=  0xDC;
    lbracket=   0xDB;
    rbracket=   0xDD;
    semicolon=  0xBA;
    apostrophe= 0xDE;
    comma=      0xBC;
    dot=        0xBE;
    slash=      0xBF;
    grave=      0xC0;
    kpslash=    0x6F;
    kpmultiply= 0x6A;
    kpminus=    0x6D;
    kpplus=     0x6B;
    kpdel=      0x2E;
    kpequal=    0x8D;
    
    lOS=        0x5B;          // left win
    rOS=        0x5C;          // right win
    menu=       0x5D;          // menu/propr

  /// direct input uses normal key codes
  } else {
    esc=        0x01; /// DIK_ESCAPE;
    enter=      0x1C; /// DIK_RETURN;
    kpenter=    0x9C; /// DIK_NUMPADENTER;
    tab=        0x0F; /// DIK_TAB;
    space=      0x39; /// DIK_SPACE;
    backspace=  0x0E; /// DIK_BACK;
    insert=     0xD2; /// DIK_INSERT;
    del=        0xD3; /// DIK_DELETE;
    home=       0xC7; /// DIK_HOME;
    end=        0xCF; /// DIK_END;
    pgup=       0xC9; /// DIK_PRIOR;
    pgdown=     0xD1; /// DIK_NEXT;
    rshift=     0x36; /// DIK_RSHIFT;
    lshift=     0x2A; /// DIK_LSHIFT;
    rctrl=      0x9D; /// DIK_RCONTROL;
    lctrl=      0x1D; /// DIK_LCONTROL;
    ralt=       0xB8; /// DIK_RMENU;
    lalt=       0x38; /// DIK_LMENU;
    f1=         0x3B; /// DIK_F1;
    f2=         0x3C; /// DIK_F2;
    f3=         0x3D; /// DIK_F3;
    f4=         0x3E; /// DIK_F4;
    f5=         0x3F; /// DIK_F5;
    f6=         0x40; /// DIK_F6;
    f7=         0x41; /// DIK_F7;
    f8=         0x42; /// DIK_F8;
    f9=         0x43; /// DIK_F9;
    f10=        0x44; /// DIK_F10;
    f11=        0x57; /// DIK_F11;
    f12=        0x58; /// DIK_F12;
    left=       0xCB; /// DIK_LEFT;
    up=         0xC8; /// DIK_UP;
    right=      0xCD; /// DIK_RIGHT;
    down=       0xD0; /// DIK_DOWN;
    prtsc=      0x2C;
    pause=      0xC5; /// DIK_PAUSE;
    capslock=   0x3A; /// DIK_CAPITAL;
    scrolllock= 0x46; /// DIK_SCROLL;
    numlock=    0x45; /// DIK_NUMLOCK;
    q=          0x10; /// DIK_Q;
    w=          0x11; /// DIK_W;
    e=          0x12; /// DIK_E;
    r=          0x13; /// DIK_R;
    t=          0x14; /// DIK_T;
    y=          0x15; /// DIK_Y;
    u=          0x16; /// DIK_U;
    i=          0x17; /// DIK_I;
    o=          0x18; /// DIK_O;
    p=          0x19; /// DIK_P;
    a=          0x1E; /// DIK_A;
    s=          0x1F; /// DIK_S;
    d=          0x20; /// DIK_D;
    f=          0x21; /// DIK_F;
    g=          0x22; /// DIK_G;
    h=          0x23; /// DIK_H;
    j=          0x24; /// DIK_J;
    k=          0x25; /// DIK_K;
    l=          0x26; /// DIK_L;
    z=          0x2C; /// DIK_Z;
    x=          0x2D; /// DIK_X;
    c=          0x2E; /// DIK_C;
    v=          0x2F; /// DIK_V;
    b=          0x30; /// DIK_B;
    n=          0x31; /// DIK_N;
    m=          0x32; /// DIK_M;
    n1=         0x02; /// DIK_1;
    n2=         0x03; /// DIK_2;
    n3=         0x04; /// DIK_3;
    n4=         0x05; /// DIK_4;
    n5=         0x06; /// DIK_5;
    n6=         0x07; /// DIK_6;
    n7=         0x08; /// DIK_7;
    n8=         0x09; /// DIK_8;
    n9=         0x0A; /// DIK_9;
    n0=         0x0B; /// DIK_0;
    kp1=        0x4F; /// DIK_NUMPAD1;
    kp2=        0x50; /// DIK_NUMPAD2;
    kp3=        0x51; /// DIK_NUMPAD3;
    kp4=        0x4B; /// DIK_NUMPAD4;
    kp5=        0x4C; /// DIK_NUMPAD5;
    kp6=        0x4D; /// DIK_NUMPAD6;
    kp7=        0x47; /// DIK_NUMPAD7;
    kp8=        0x48; /// DIK_NUMPAD8;
    kp9=        0x49; /// DIK_NUMPAD9;
    kp0=        0x52; /// DIK_NUMPAD0;
    minus=      0x0C; /// DIK_MINUS;
    equal=      0x0D; /// DIK_EQUALS;
    backslash=  0x2B; /// DIK_BACKSLASH;
    lbracket=   0x1A; /// DIK_LBRACKET;
    rbracket=   0x1B; /// DIK_RBRACKET;
    semicolon=  0x27; /// DIK_SEMICOLON;
    apostrophe= 0x28; /// DIK_APOSTROPHE;
    comma=      0x33; /// DIK_COMMA;
    dot=        0x34; /// DIK_PERIOD;
    slash=      0x35; /// DIK_SLASH;
    grave=      0x29; /// DIK_GRAVE;
    kpslash=    0xB5; /// DIK_DIVIDE;
    kpmultiply= 0x37; /// DIK_MULTIPLY;
    kpminus=    0x4A; /// DIK_SUBTRACT;
    kpplus=     0x4E; /// DIK_ADD;
    kpdel=      0xB3; /// DIK_NUMPADCOMMA;
    kpequal=    0x8D; /// DIK_NUMPADEQUALS;
    
    lOS=        0xDB; /// DIK_LWIN;          // left win
    rOS=        0xDC; /// DIK_RWIN;          // right win
    menu=       0x5D;                        // menu/propr
  } /// MODE1 / MODE3
  #endif /// OS_WIN
    
  #ifdef OS_LINUX
  esc= XKeysymToKeycode(osi.primWin->_dis, XK_Escape);
  enter= XKeysymToKeycode(osi.primWin->_dis, XK_Return);
  tab= XKeysymToKeycode(osi.primWin->_dis, XK_Tab);
  space= XKeysymToKeycode(osi.primWin->_dis, XK_space);
  backspace= XKeysymToKeycode(osi.primWin->_dis, XK_BackSpace);
  insert= XKeysymToKeycode(osi.primWin->_dis, XK_Insert);
  del= XKeysymToKeycode(osi.primWin->_dis, XK_Delete);
  home= XKeysymToKeycode(osi.primWin->_dis, XK_Home);
  end= XKeysymToKeycode(osi.primWin->_dis, XK_End);
  pgup= XKeysymToKeycode(osi.primWin->_dis, XK_Page_Up);
  pgdown= XKeysymToKeycode(osi.primWin->_dis, XK_Page_Down);
  rshift= XKeysymToKeycode(osi.primWin->_dis, XK_Shift_R);
  lshift= XKeysymToKeycode(osi.primWin->_dis, XK_Shift_L);
  rctrl= XKeysymToKeycode(osi.primWin->_dis, XK_Control_R);
  lctrl= XKeysymToKeycode(osi.primWin->_dis, XK_Control_L);
  ralt= XKeysymToKeycode(osi.primWin->_dis, XK_Alt_R);
  lalt= XKeysymToKeycode(osi.primWin->_dis, XK_Alt_L);
  f1= XKeysymToKeycode(osi.primWin->_dis, XK_F1);
  f2= XKeysymToKeycode(osi.primWin->_dis, XK_F2);
  f3= XKeysymToKeycode(osi.primWin->_dis, XK_F3);
  f4= XKeysymToKeycode(osi.primWin->_dis, XK_F4);
  f5= XKeysymToKeycode(osi.primWin->_dis, XK_F5);
  f6= XKeysymToKeycode(osi.primWin->_dis, XK_F6);
  f7= XKeysymToKeycode(osi.primWin->_dis, XK_F7);
  f8= XKeysymToKeycode(osi.primWin->_dis, XK_F8);
  f9= XKeysymToKeycode(osi.primWin->_dis, XK_F9);
  f10= XKeysymToKeycode(osi.primWin->_dis, XK_F10);
  f11= XKeysymToKeycode(osi.primWin->_dis, XK_F11);
  f12= XKeysymToKeycode(osi.primWin->_dis, XK_F12);
  left= XKeysymToKeycode(osi.primWin->_dis, XK_Left);
  up= XKeysymToKeycode(osi.primWin->_dis, XK_Up);
  right= XKeysymToKeycode(osi.primWin->_dis, XK_Right);
  down= XKeysymToKeycode(osi.primWin->_dis, XK_Down);
  prtsc= XKeysymToKeycode(osi.primWin->_dis, XK_Print);
  pause= XKeysymToKeycode(osi.primWin->_dis, XK_Pause);
  capslock= XKeysymToKeycode(osi.primWin->_dis, XK_Caps_Lock);
  scrolllock= XKeysymToKeycode(osi.primWin->_dis, XK_Scroll_Lock);
  numlock= XKeysymToKeycode(osi.primWin->_dis, XK_Num_Lock);
  q= XKeysymToKeycode(osi.primWin->_dis, XK_q);
  w= XKeysymToKeycode(osi.primWin->_dis, XK_w);
  e= XKeysymToKeycode(osi.primWin->_dis, XK_e);
  r= XKeysymToKeycode(osi.primWin->_dis, XK_r);
  t= XKeysymToKeycode(osi.primWin->_dis, XK_t);
  y= XKeysymToKeycode(osi.primWin->_dis, XK_y);
  u= XKeysymToKeycode(osi.primWin->_dis, XK_u);
  i= XKeysymToKeycode(osi.primWin->_dis, XK_i);
  o= XKeysymToKeycode(osi.primWin->_dis, XK_o);
  p= XKeysymToKeycode(osi.primWin->_dis, XK_p);
  a= XKeysymToKeycode(osi.primWin->_dis, XK_a);
  s= XKeysymToKeycode(osi.primWin->_dis, XK_s);
  d= XKeysymToKeycode(osi.primWin->_dis, XK_d);
  f= XKeysymToKeycode(osi.primWin->_dis, XK_f);
  g= XKeysymToKeycode(osi.primWin->_dis, XK_g);
  h= XKeysymToKeycode(osi.primWin->_dis, XK_h);
  j= XKeysymToKeycode(osi.primWin->_dis, XK_j);
  k= XKeysymToKeycode(osi.primWin->_dis, XK_k);
  l= XKeysymToKeycode(osi.primWin->_dis, XK_l);
  z= XKeysymToKeycode(osi.primWin->_dis, XK_z);
  x= XKeysymToKeycode(osi.primWin->_dis, XK_x);
  c= XKeysymToKeycode(osi.primWin->_dis, XK_c);
  v= XKeysymToKeycode(osi.primWin->_dis, XK_v);
  b= XKeysymToKeycode(osi.primWin->_dis, XK_b);
  n= XKeysymToKeycode(osi.primWin->_dis, XK_n);
  m= XKeysymToKeycode(osi.primWin->_dis, XK_m);
  n1= XKeysymToKeycode(osi.primWin->_dis, XK_1);
  n2= XKeysymToKeycode(osi.primWin->_dis, XK_2);
  n3= XKeysymToKeycode(osi.primWin->_dis, XK_3);
  n4= XKeysymToKeycode(osi.primWin->_dis, XK_4);
  n5= XKeysymToKeycode(osi.primWin->_dis, XK_5);
  n6= XKeysymToKeycode(osi.primWin->_dis, XK_6);
  n7= XKeysymToKeycode(osi.primWin->_dis, XK_7);
  n8= XKeysymToKeycode(osi.primWin->_dis, XK_8);
  n9= XKeysymToKeycode(osi.primWin->_dis, XK_9);
  n0= XKeysymToKeycode(osi.primWin->_dis, XK_0);
  minus= XKeysymToKeycode(osi.primWin->_dis, XK_minus);
  equal= XKeysymToKeycode(osi.primWin->_dis, XK_equal);
  backslash= XKeysymToKeycode(osi.primWin->_dis, XK_backslash);
  lbracket= XKeysymToKeycode(osi.primWin->_dis, XK_bracketleft);
  rbracket= XKeysymToKeycode(osi.primWin->_dis, XK_bracketright);
  semicolon= XKeysymToKeycode(osi.primWin->_dis, XK_semicolon);
  apostrophe= XKeysymToKeycode(osi.primWin->_dis, XK_apostrophe);
  comma= XKeysymToKeycode(osi.primWin->_dis, XK_comma);
  dot= XKeysymToKeycode(osi.primWin->_dis, XK_period);
  slash= XKeysymToKeycode(osi.primWin->_dis, XK_slash);
  grave= XKeysymToKeycode(osi.primWin->_dis, XK_grave);
  kp1= XKeysymToKeycode(osi.primWin->_dis, XK_KP_1);
  kp2= XKeysymToKeycode(osi.primWin->_dis, XK_KP_2);
  kp3= XKeysymToKeycode(osi.primWin->_dis, XK_KP_3);
  kp4= XKeysymToKeycode(osi.primWin->_dis, XK_KP_4);
  kp5= XKeysymToKeycode(osi.primWin->_dis, XK_KP_5);
  kp6= XKeysymToKeycode(osi.primWin->_dis, XK_KP_6);
  kp7= XKeysymToKeycode(osi.primWin->_dis, XK_KP_7);
  kp8= XKeysymToKeycode(osi.primWin->_dis, XK_KP_8);
  kp9= XKeysymToKeycode(osi.primWin->_dis, XK_KP_9);
  kp0= XKeysymToKeycode(osi.primWin->_dis, XK_KP_0);
  kpequal= XKeysymToKeycode(osi.primWin->_dis, XK_KP_Equal);
  kpslash= XKeysymToKeycode(osi.primWin->_dis, XK_KP_Divide);
  kpmultiply= XKeysymToKeycode(osi.primWin->_dis, XK_KP_Multiply);
  kpminus= XKeysymToKeycode(osi.primWin->_dis, XK_KP_Subtract);
  kpplus= XKeysymToKeycode(osi.primWin->_dis, XK_KP_Add);
  kpenter= XKeysymToKeycode(osi.primWin->_dis, XK_KP_Enter);
  kpdel= XKeysymToKeycode(osi.primWin->_dis, XK_KP_Delete);
  lOS= XKeysymToKeycode(osi.primWin->_dis, XK_Super_L);           // left win
  rOS= XKeysymToKeycode(osi.primWin->_dis, XK_Super_R);           // right win
  menu= XKeysymToKeycode(osi.primWin->_dis, XK_Menu);             // menu/propr
  
  #endif /// OS_LINUX
    
  #ifdef OS_MAC
  
  // THE FOLLOWING ARE NOT HANDLED !!!!
  /*
   // JIS keyboards only
	enum {
	  kVK_JIS_Yen                   = 0x5D,
	  kVK_JIS_Underscore            = 0x5E,
	  kVK_JIS_KeypadComma           = 0x5F,
	  kVK_JIS_Eisu                  = 0x66,
	  kVK_JIS_Kana                  = 0x68
	};
  */
  
  // http://forums.macrumors.com/showthread.php?t=780577
  
  esc=        0x35; // kVK_Escape
  enter=      0x24; // kVK_Return
  kpenter=    0x4C; // kVK_ANSI_KeypadEnter
  tab=        0x30; // kVK_Tab
  space=      0x31; // kVK_Space
  backspace=  0x33; // kVK_Delete
  insert=     0x72; // online doc says 0x3F. seems WRONG...;
  del=        0x75; // kVK_ForwardDelete
  home=       0x73; // kVK_Home
  end=        0x77; // kVK_End
  pgup=       0x74; // kVK_PageUp
  pgdown=     0x79; // kVK_PageDown
  rshift=     0x3C; // kVK_RightShift
  lshift=     0x38; // kVK_Shift
  rctrl=      0x3E; // kVK_RightControl
  lctrl=      0x3B; // kVK_Control
  ralt=       0x3D; // kVK_RightOption
  lalt=       0x3A; // kVK_Option
  f1=         0x7A; // kVK_F1
  f2=         0x78; // kVK_F2
  f3=         0x63; // kVK_F3
  f4=         0x76; // kVK_F4
  f5=         0x60; // kVK_F5
  f6=         0x61; // kVK_F6
  f7=         0x62; // kVK_F7
  f8=         0x64; // kVK_F8
  f9=         0x65; // kVK_F9
  f10=        0x6D; // kVK_F10
  f11=        0x67; // kVK_F11                  SYSTEM GRABED BY DEFAULT
  f12=        0x6F; // kVK_F12                  .SYSTEM GRABED BY DEFAULT
  left=       0x7B; // kVK_LeftArrow
  up=         0x7E; // kVK_UpArrow
  right=      0x7C; // kVK_RightArrow
  down=       0x7D; // kVK_DownArrow
  prtsc=      0x69; // kVK_F13
  pause=      0x71; // kVK_F15
  capslock=   0x39; // kVK_CapsLock
  scrolllock= 0x6B; // kVK_F14
  numlock=    0x47; // kVK_ANSI_KeypadClear
  q=          0x0C; // kVK_ANSI_Q
  w=          0x0D; // kVK_ANSI_W
  e=          0x0E; // kVK_ANSI_E
  r=          0x0F; // kVK_ANSI_R
  t=          0x11; // kVK_ANSI_T
  y=          0x10; // kVK_ANSI_Y
  u=          0x20; // kVK_ANSI_U
  i=          0x22; // kVK_ANSI_I
  o=          0x1F; // kVK_ANSI_O
  p=          0x23; // kVK_ANSI_P
  a=          0x00; // kVK_ANSI_A
  s=          0x01; // kVK_ANSI_S
  d=          0x02; // kVK_ANSI_D
  f=          0x03; // kVK_ANSI_F
  g=          0x05; // kVK_ANSI_G
  h=          0x04; // kVK_ANSI_H
  j=          0x26; // kVK_ANSI_J
  k=          0x28; // kVK_ANSI_K
  l=          0x25; // kVK_ANSI_L
  z=          0x06; // kVK_ANSI_Z
  x=          0x07; // kVK_ANSI_X
  c=          0x08; // kVK_ANSI_C
  v=          0x09; // kVK_ANSI_V
  b=          0x0B; // kVK_ANSI_B
  n=          0x2D; // kVK_ANSI_N
  m=          0x2E; // kVK_ANSI_M
  n1=         0x12; // kVK_ANSI_1
  n2=         0x13; // kVK_ANSI_2
  n3=         0x14; // kVK_ANSI_3
  n4=         0x15; // kVK_ANSI_4
  n5=         0x17; // kVK_ANSI_5
  n6=         0x16; // kVK_ANSI_6
  n7=         0x1A; // kVK_ANSI_7
  n8=         0x1C; // kVK_ANSI_8
  n9=         0x19; // kVK_ANSI_9
  n0=         0x1D; // kVK_ANSI_0
  kp1=        0x53; // kVK_ANSI_Keypad1
  kp2=        0x54; // kVK_ANSI_Keypad2
  kp3=        0x55; // kVK_ANSI_Keypad3
  kp4=        0x56; // kVK_ANSI_Keypad4
  kp5=        0x57; // kVK_ANSI_Keypad5
  kp6=        0x58; // kVK_ANSI_Keypad6
  kp7=        0x59; // kVK_ANSI_Keypad7
  kp8=        0x5B; // kVK_ANSI_Keypad8
  kp9=        0x5C; // kVK_ANSI_Keypad9
  kp0=        0x52; // kVK_ANSI_Keypad0
  minus=      0x1B; // kVK_ANSI_Minus
  equal=      0x18; // kVK_ANSI_Equal
  backslash=  0x2A; // kVK_ANSI_Backslash
  lbracket=   0x21; // kVK_ANSI_LeftBracket
  rbracket=   0x1E; // kVK_ANSI_RightBracket
  semicolon=  0x29; // kVK_ANSI_Semicolon
  apostrophe= 0x27; // kVK_ANSI_Quote
  comma=      0x2B; // kVK_ANSI_Comma
  dot=        0x2F; // kVK_ANSI_Period
  slash=      0x2C; // kVK_ANSI_Slash
  grave=      0x32; // kVK_ANSI_Grave
  kpequal=    0x51; // kVK_ANSI_KeypadEquals
  kpslash=    0x4B; // kVK_ANSI_KeypadDivide
  kpmultiply= 0x43; // kVK_ANSI_KeypadMultiply
  kpminus=    0x4E; // kVK_ANSI_KeypadMinus
  kpplus=     0x45; // kVK_ANSI_KeypadPlus
  kpdel=      0x41; // kVK_ANSI_KeypadDecimal
  lOS=        0x37; // kVK_Command                left win
  rOS=        0x36; // ??? found via test         right win
  menu=       0x6E; // ??? found via test         menu/propr

  #endif /// OS_MAC
}





