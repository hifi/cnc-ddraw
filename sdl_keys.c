#include "sdl_keys.h"

#define VK_OEM_PLUS     0xBB
#define VK_OEM_COMMA    0xBC
#define VK_OEM_MINUS    0xBD
#define VK_OEM_PERIOD   0xBE

int VKey_to_SDLKey[VK_KEYS] =
{
    0,                      // 0x00 Undefined
    0,                      // 0x01 VK_LBUTTON
    0,                      // 0x02 VK_RBUTTON
    0,                      // 0x03 VK_CANCEL
    0,                      // 0x04 VK_MBUTTON
    0,                      // 0x05 VK_XBUTTON1
    0,                      // 0x06 VK_XBUTTON2
    0,                      // 0x07 Undefined
    SDLK_BACKSPACE,         // 0x08 VK_BACK
    SDLK_TAB,               // 0x09 VK_TAB
    0,                      // 0x0A Reserved
    0,                      // 0x0B Reserved
    0,                      // 0x0C VK_CLEAR
    SDLK_RETURN,            // 0x0D VK_RETURN
    0,                      // 0x0E Undefined
    0,                      // 0x0F Undefined
    0,                      // 0x10 VK_SHIFT
    0,                      // 0x11 VK_CONTROL
    0,                      // 0x12 VK_MENU
    SDLK_PAUSE,             // 0x13 VK_PAUSE
    0,                      // 0x14 VK_CAPITAL
    0,                      // 0x15 VK_KANA | VK_HANGUEL | VK_HANGUL
    0,                      // 0x16 Undefined
    0,                      // 0x17 VK_JUNJA
    0,                      // 0x18 VK_FINAL
    0,                      // 0x19 VK_HANJA | VK_KANJI
    0,                      // 0x1A Undefined
    SDLK_ESCAPE,            // 0x1B VK_ESCAPE
    0,                      // 0x1C 
    0,                      // 0x1D 
    0,                      // 0x1E 
    0,                      // 0x1F 
    0,                      // 0x20 
    0,                      // 0x21 
    0,                      // 0x22 
    0,                      // 0x23 
    0,                      // 0x24 
    0,                      // 0x25 
    0,                      // 0x26 
    0,                      // 0x27 
    0,                      // 0x28 
    0,                      // 0x29 
    0,                      // 0x2A 
    0,                      // 0x2B 
    SDLK_PRINT,             // 0x2C VK_SNAPSHOT
    SDLK_INSERT,            // 0x2D INSERT
    SDLK_DELETE,            // 0x2E DELETE
    0,                      // 0x2F 
    48,                     // 0x30 0
    49,                     // 0x31 1
    50,                     // 0x32 2
    51,                     // 0x33 3
    52,                     // 0x34 4
    53,                     // 0x35 5
    54,                     // 0x36 6
    55,                     // 0x37 7
    56,                     // 0x38 8
    57,                     // 0x39 9
    0,                      // 0x3A 
    0,                      // 0x3B 
    0,                      // 0x3C 
    0,                      // 0x3D 
    0,                      // 0x3E 
    0,                      // 0x3F 
    0,                      // 0x40 
    SDLK_a,                 // 0x41 a
    SDLK_b,                 // 0x42 b
    SDLK_c,                 // 0x43 c
    SDLK_d,                 // 0x44 d
    SDLK_e,                 // 0x45 e
    SDLK_f,                 // 0x46 f
    SDLK_g,                 // 0x47 g
    SDLK_h,                 // 0x48 h
    SDLK_i,                 // 0x49 i
    SDLK_j,                 // 0x4A j
    SDLK_k,                 // 0x4B k
    SDLK_l,                 // 0x4C l
    SDLK_m,                 // 0x4D m
    SDLK_n,                 // 0x4E n
    SDLK_o,                 // 0x4F o
    SDLK_p,                 // 0x50 p
    SDLK_q,                 // 0x51 q
    SDLK_r,                 // 0x52 r
    SDLK_s,                 // 0x53 s
    SDLK_t,                 // 0x54 t
    SDLK_u,                 // 0x55 u
    SDLK_v,                 // 0x56 v
    SDLK_w,                 // 0x57 w
    SDLK_x,                 // 0x58 x
    SDLK_y,                 // 0x59 y
    SDLK_z,                 // 0x5A z
    0,                      // 0x5B 
    0,                      // 0x5C 
    0,                      // 0x5D 
    0,                      // 0x5E 
    0,                      // 0x5F 
    0,                      // 0x60 
    0,                      // 0x61 
    0,                      // 0x62 
    0,                      // 0x63 
    0,                      // 0x64 
    0,                      // 0x65 
    0,                      // 0x66 
    0,                      // 0x67 
    0,                      // 0x68 
    0,                      // 0x69 
    0,                      // 0x6A 
    0,                      // 0x6B 
    0,                      // 0x6C 
    0,                      // 0x6D 
    0,                      // 0x6E 
    0,                      // 0x6F 
    SDLK_F1,                // 0x70 F1
    SDLK_F2,                // 0x71 F2
    SDLK_F3,                // 0x72 F3
    SDLK_F4,                // 0x73 F4
    SDLK_F5,                // 0x74 F5
    SDLK_F6,                // 0x75 F6
    SDLK_F7,                // 0x76 F7
    SDLK_F8,                // 0x77 F8
    SDLK_F9,                // 0x78 F9
    SDLK_F10,               // 0x79 F10
    SDLK_F11,               // 0x7A F11
    SDLK_F12,               // 0x7B F12
    SDLK_F13,               // 0x7C F13
    SDLK_F14,               // 0x7D F14
    SDLK_F15,               // 0x7E F15
    0,                      // 0x7F F16
    0,                      // 0x80 F17
    0,                      // 0x81 F18
    0,                      // 0x82 F19
    0,                      // 0x83 F20
    0,                      // 0x84 F21
    0,                      // 0x85 F22
    0,                      // 0x86 F23
    0,                      // 0x87 F24
    0,                      // 0x88 
    0,                      // 0x89 
    0,                      // 0x8A 
    0,                      // 0x8B 
    0,                      // 0x8C 
    0,                      // 0x8D 
    0,                      // 0x8E 
    0,                      // 0x8F 
    0,                      // 0x90 
    0,                      // 0x91 
    0,                      // 0x92 
    0,                      // 0x93 
    0,                      // 0x94 
    0,                      // 0x95 
    0,                      // 0x96 
    0,                      // 0x97 
    0,                      // 0x98 
    0,                      // 0x99 
    0,                      // 0x9A 
    0,                      // 0x9B 
    0,                      // 0x9C 
    0,                      // 0x9D 
    0,                      // 0x9E 
    0,                      // 0x9F 
    0,                      // 0xA0 
    0,                      // 0xA1 
    0,                      // 0xA2 
    0,                      // 0xA3 
    0,                      // 0xA4 
    0,                      // 0xA5 
    0,                      // 0xA6 
    0,                      // 0xA7 
    0,                      // 0xA8 
    0,                      // 0xA9 
    0,                      // 0xAA 
    0,                      // 0xAB 
    0,                      // 0xAC 
    0,                      // 0xAD 
    0,                      // 0xAE 
    0,                      // 0xAF 
    0,                      // 0xB0 
    0,                      // 0xB1 
    0,                      // 0xB2 
    0,                      // 0xB3 
    0,                      // 0xB4 
    0,                      // 0xB5 
    0,                      // 0xB6 
    0,                      // 0xB7 
    0,                      // 0xB8 
    0,                      // 0xB9 
    0,                      // 0xBA 
    0,                      // 0xBB 
    0,                      // 0xBC 
    0,                      // 0xBD 
    0,                      // 0xBE 
    0,                      // 0xBF 
    0,                      // 0xC0 
    0,                      // 0xC1 
    0,                      // 0xC2 
    0,                      // 0xC3 
    0,                      // 0xC4 
    0,                      // 0xC5 
    0,                      // 0xC6 
    0,                      // 0xC7 
    0,                      // 0xC8 
    0,                      // 0xC9 
    0,                      // 0xCA 
    0,                      // 0xCB 
    0,                      // 0xCC 
    0,                      // 0xCD 
    0,                      // 0xCE 
    0,                      // 0xCF 
    0,                      // 0xD0 
    0,                      // 0xD1 
    0,                      // 0xD2 
    0,                      // 0xD3 
    0,                      // 0xD4 
    0,                      // 0xD5 
    0,                      // 0xD6 
    0,                      // 0xD7 
    0,                      // 0xD8 
    0,                      // 0xD9 
    0,                      // 0xDA 
    0,                      // 0xDB 
    0,                      // 0xDC 
    0,                      // 0xDD 
    0,                      // 0xDE 
    0,                      // 0xDF 
    0,                      // 0xE0 
    0,                      // 0xE1 
    0,                      // 0xE2 
    0,                      // 0xE3 
    0,                      // 0xE4 
    0,                      // 0xE5 
    0,                      // 0xE6 
    0,                      // 0xE7 
    0,                      // 0xE8 
    0,                      // 0xE9 
    0,                      // 0xEA 
    0,                      // 0xEB 
    0,                      // 0xEC 
    0,                      // 0xED 
    0,                      // 0xEE 
    0,                      // 0xEF 
    0,                      // 0xF0 
    0,                      // 0xF1 
    0,                      // 0xF2 
    0,                      // 0xF3 
    0,                      // 0xF4 
    0,                      // 0xF5 
    0,                      // 0xF6 
    0,                      // 0xF7 
    0,                      // 0xF8 
    0,                      // 0xF9 
    0,                      // 0xFA 
    0,                      // 0xFB 
    0,                      // 0xFC 
    0,                      // 0xFD 
    0,                      // 0xFE 
    0                       // 0xFF 
};

Uint8 SDLKey_to_VKey[SDL_KEYS] =
{
    0,                      //    0 
    0,                      //    1 
    0,                      //    2 
    0,                      //    3 
    0,                      //    4 
    0,                      //    5 
    0,                      //    6 
    0,                      //    7 
    VK_BACK,                //    8 SDLK_BACKSPACE
    VK_TAB,                 //    9 SDLK_TAB
    0,                      //   10 
    0,                      //   11 
    0,                      //   12 
    VK_RETURN,              //   13 SDLK_RETURN
    0,                      //   14 
    0,                      //   15 
    0,                      //   16 
    0,                      //   17 
    0,                      //   18 
    VK_PAUSE,               //   19 SDLK_PAUSE
    0,                      //   20 
    0,                      //   21 
    0,                      //   22 
    0,                      //   23 
    0,                      //   24 
    0,                      //   25 
    0,                      //   26 
    VK_ESCAPE,              //   27 SDLK_ESCAPE
    0,                      //   28 
    0,                      //   29 
    0,                      //   30 
    0,                      //   31 
    0,                      //   32 
    0,                      //   33 
    0,                      //   34 
    0,                      //   35 
    0,                      //   36 
    0,                      //   37 
    0,                      //   38 
    VK_OEM_7,               //   39 SDLK_QUOTE
    0,                      //   40 
    0,                      //   41 
    0,                      //   42 
    0,                      //   43 
    VK_OEM_COMMA,           //   44 SDLK_COMMA
    VK_OEM_MINUS,           //   45 SDLK_MINUS
    VK_OEM_PERIOD,          //   46 SDLK_PERIOD
    VK_OEM_2,               //   47 SDLK_SLASH,
    0x30,                   //   48 0
    0x31,                   //   49 1
    0x32,                   //   50 2
    0x33,                   //   51 3
    0x34,                   //   52 4
    0x35,                   //   53 5
    0x36,                   //   54 6
    0x37,                   //   55 7
    0x38,                   //   56 8
    0x39,                   //   57 9
    0,                      //   58 
    VK_OEM_1,               //   59 SDLK_SEMICOLON
    0,                      //   60 
    VK_OEM_PLUS,            //   61 SDLK_EQUALS
    0,                      //   62 
    0,                      //   63 
    0,                      //   64 
    0,                      //   65 
    0,                      //   66 
    0,                      //   67 
    0,                      //   68 
    0,                      //   69 
    0,                      //   70 
    0,                      //   71 
    0,                      //   72 
    0,                      //   73 
    0,                      //   74 
    0,                      //   75 
    0,                      //   76 
    0,                      //   77 
    0,                      //   78 
    0,                      //   79 
    0,                      //   80 
    0,                      //   81 
    0,                      //   82 
    0,                      //   83 
    0,                      //   84 
    0,                      //   85 
    0,                      //   86 
    0,                      //   87 
    0,                      //   88 
    0,                      //   89 
    0,                      //   90 
    VK_OEM_4,               //   91 SDLK_LEFTBRACKET
    VK_OEM_5,               //   92 SDLK_BACKSLASH
    VK_OEM_6,               //   93 SDLK_RIGHTBRACKET
    0,                      //   94 SDLK_CARET
    0,                      //   95 SDLK_UNDERSCORE
    VK_OEM_3,               //   96 SDLK_BACKQUOTE
    0x41,                   //   97 a
    0x42,                   //   98 b
    0x43,                   //   99 c
    0x44,                   //  100 d
    0x45,                   //  101 e
    0x46,                   //  102 f
    0x47,                   //  103 g
    0x48,                   //  104 h
    0x49,                   //  105 i
    0x4A,                   //  106 j
    0x4B,                   //  107 k
    0x4C,                   //  108 l
    0x4D,                   //  109 m
    0x4E,                   //  110 n
    0x4F,                   //  111 o
    0x50,                   //  112 p
    0x51,                   //  113 q
    0x52,                   //  114 r
    0x53,                   //  115 s
    0x54,                   //  116 t
    0x55,                   //  117 u
    0x56,                   //  118 v
    0x57,                   //  119 w
    0x58,                   //  120 x
    0x59,                   //  121 y
    0x5A,                   //  122 z
    0,                      //  123 
    0,                      //  124 
    0,                      //  125 
    0,                      //  126 
    VK_DELETE,              //  127 SDLK_DELETE
    0,                      //  128 
    0,                      //  129 
    0,                      //  130 
    0,                      //  131 
    0,                      //  132 
    0,                      //  133 
    0,                      //  134 
    0,                      //  135 
    0,                      //  136 
    0,                      //  137 
    0,                      //  138 
    0,                      //  139 
    0,                      //  140 
    0,                      //  141 
    0,                      //  142 
    0,                      //  143 
    0,                      //  144 
    0,                      //  145 
    0,                      //  146 
    0,                      //  147 
    0,                      //  148 
    0,                      //  149 
    0,                      //  150 
    0,                      //  151 
    0,                      //  152 
    0,                      //  153 
    0,                      //  154 
    0,                      //  155 
    0,                      //  156 
    0,                      //  157 
    0,                      //  158 
    0,                      //  159 
    0,                      //  160 
    0,                      //  161 
    0,                      //  162 
    0,                      //  163 
    0,                      //  164 
    0,                      //  165 
    0,                      //  166 
    0,                      //  167 
    0,                      //  168 
    0,                      //  169 
    0,                      //  170 
    0,                      //  171 
    0,                      //  172 
    0,                      //  173 
    0,                      //  174 
    0,                      //  175 
    0,                      //  176 
    0,                      //  177 
    0,                      //  178 
    0,                      //  179 
    0,                      //  180 
    0,                      //  181 
    0,                      //  182 
    0,                      //  183 
    0,                      //  184 
    0,                      //  185 
    0,                      //  186 
    0,                      //  187 
    0,                      //  188 
    0,                      //  189 
    0,                      //  190 
    0,                      //  191 
    0,                      //  192 
    0,                      //  193 
    0,                      //  194 
    0,                      //  195 
    0,                      //  196 
    0,                      //  197 
    0,                      //  198 
    0,                      //  199 
    0,                      //  200 
    0,                      //  201 
    0,                      //  202 
    0,                      //  203 
    0,                      //  204 
    0,                      //  205 
    0,                      //  206 
    0,                      //  207 
    0,                      //  208 
    0,                      //  209 
    0,                      //  210 
    0,                      //  211 
    0,                      //  212 
    0,                      //  213 
    0,                      //  214 
    0,                      //  215 
    0,                      //  216 
    0,                      //  217 
    0,                      //  218 
    0,                      //  219 
    0,                      //  220 
    0,                      //  221 
    0,                      //  222 
    0,                      //  223 
    0,                      //  224 
    0,                      //  225 
    0,                      //  226 
    0,                      //  227 
    0,                      //  228 
    0,                      //  229 
    0,                      //  230 
    0,                      //  231 
    0,                      //  232 
    0,                      //  233 
    0,                      //  234 
    0,                      //  235 
    0,                      //  236 
    0,                      //  237 
    0,                      //  238 
    0,                      //  239 
    0,                      //  240 
    0,                      //  241 
    0,                      //  242 
    0,                      //  243 
    0,                      //  244 
    0,                      //  245 
    0,                      //  246 
    0,                      //  247 
    0,                      //  248 
    0,                      //  249 
    0,                      //  250 
    0,                      //  251 
    0,                      //  252 
    0,                      //  253 
    0,                      //  254 
    0,                      //  255 
    0,                      //  256 
    0,                      //  257 
    0,                      //  258 
    0,                      //  259 
    0,                      //  260 
    0,                      //  261 
    0,                      //  262 
    0,                      //  263 
    0,                      //  264 
    0,                      //  265 
    0,                      //  266 
    0,                      //  267 
    0,                      //  268 
    0,                      //  269 
    0,                      //  270 
    0,                      //  271 
    0,                      //  272 
    VK_UP,                  //  273 SDLK_UP
    VK_DOWN,                //  274 SDLK_DOWN
    VK_RIGHT,               //  275 SDLK_RIGHT
    VK_LEFT,                //  276 SDLK_LEFT
    VK_INSERT,              //  277 SDLK_INSERT
    VK_HOME,                //  278 SDLK_HOME
    VK_END,                 //  279 SDLK_END
    VK_PRIOR,               //  280 SDLK_PAGEUP
    VK_NEXT,                //  281 SDLK_PAGEDOWN
    VK_F1,                  //  282 F1
    VK_F2,                  //  283 F2
    VK_F3,                  //  284 F3
    VK_F4,                  //  285 F4
    VK_F5,                  //  286 F5
    VK_F6,                  //  287 F6
    VK_F7,                  //  288 F7
    VK_F8,                  //  289 F8
    VK_F9,                  //  290 F9
    VK_F10,                 //  291 F10
    VK_F11,                 //  292 F11
    VK_F12,                 //  293 F12
    VK_F13,                 //  294 F13
    VK_F14,                 //  295 F14
    VK_F15,                 //  296 F15
    0,                      //  297 
    0,                      //  298 
    0,                      //  299 
    VK_NUMLOCK,             //  300 SDLK_NUMLOCK
    VK_CAPITAL,             //  301 SDLK_CAPSLOCK
    VK_SCROLL,              //  302 SDLK_SCROLLOCK
    VK_SHIFT,               //  303 SDLK_RSHIFT
    VK_SHIFT,               //  304 SDLK_LSHIFT
    VK_CONTROL,             //  305 SDLK_RCTRL
    VK_CONTROL,             //  306 SDLK_LCTRL
    VK_MENU,                //  307 SDLK_RALT
    VK_MENU,                //  308 SDLK_LALT
    VK_MENU,                //  309 SDLK_RMETA
    VK_MENU,                //  310 SDLK_LMETA
    0,                      //  311 SDLK_LSUPER
    0,                      //  312 SDLK_RSUPER
    0,                      //  313 SDLK_MODE
    0,                      //  314 SDLK_COMPOSE
    0,                      //  315 
    0,                      //  316 
    0,                      //  317 
    0,                      //  318 
    0,                      //  319 
    0,                      //  320 
    0,                      //  321 
    0                       //  322 
};
