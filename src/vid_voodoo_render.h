#if !(defined i386 || defined __i386 || defined __i386__ || defined _X86_ || defined WIN32 || defined _WIN32 || defined _WIN32) && !(defined __amd64__)
#define NO_CODEGEN
#endif

#ifndef NO_CODEGEN
void voodoo_codegen_init(voodoo_t *voodoo);
void voodoo_codegen_close(voodoo_t *voodoo);
#endif

#define DEPTH_TEST(comp_depth)                          \
        do                                              \
        {                                               \
                switch (depth_op)                       \
                {                                       \
                        case DEPTHOP_NEVER:             \
                        voodoo->fbiZFuncFail++;         \
                        goto skip_pixel;                \
                        case DEPTHOP_LESSTHAN:          \
                        if (!(comp_depth < old_depth))  \
                        {                               \
                                voodoo->fbiZFuncFail++; \
                                goto skip_pixel;        \
                        }                               \
                        break;                          \
                        case DEPTHOP_EQUAL:             \
                        if (!(comp_depth == old_depth)) \
                        {                               \
                                voodoo->fbiZFuncFail++; \
                                goto skip_pixel;        \
                        }                               \
                        break;                          \
                        case DEPTHOP_LESSTHANEQUAL:     \
                        if (!(comp_depth <= old_depth)) \
                        {                               \
                                voodoo->fbiZFuncFail++; \
                                goto skip_pixel;        \
                        }                               \
                        break;                          \
                        case DEPTHOP_GREATERTHAN:       \
                        if (!(comp_depth > old_depth))  \
                        {                               \
                                voodoo->fbiZFuncFail++; \
                                goto skip_pixel;        \
                        }                               \
                        break;                          \
                        case DEPTHOP_NOTEQUAL:          \
                        if (!(comp_depth != old_depth)) \
                        {                               \
                                voodoo->fbiZFuncFail++; \
                                goto skip_pixel;        \
                        }                               \
                        break;                          \
                        case DEPTHOP_GREATERTHANEQUAL:  \
                        if (!(comp_depth >= old_depth)) \
                        {                               \
                                voodoo->fbiZFuncFail++; \
                                goto skip_pixel;        \
                        }                               \
                        break;                          \
                        case DEPTHOP_ALWAYS:            \
                        break;                          \
                }                                       \
        } while (0)

#define APPLY_FOG(src_r, src_g, src_b, z, ia, w)                        \
        do                                                              \
        {                                                               \
                if (params->fogMode & FOG_CONSTANT)                     \
                {                                                       \
                        src_r += params->fogColor.r;                    \
                        src_g += params->fogColor.g;                    \
                        src_b += params->fogColor.b;                    \
                }                                                       \
                else                                                    \
                {                                                       \
                        int fog_r, fog_g, fog_b, fog_a = 0;             \
                        int fog_idx;                                    \
                                                                        \
                        if (!(params->fogMode & FOG_ADD))               \
                        {                                               \
                                fog_r = params->fogColor.r;             \
                                fog_g = params->fogColor.g;             \
                                fog_b = params->fogColor.b;             \
                        }                                               \
                        else                                            \
                                fog_r = fog_g = fog_b = 0;              \
                                                                        \
                        if (!(params->fogMode & FOG_MULT))              \
                        {                                               \
                                fog_r -= src_r;                         \
                                fog_g -= src_g;                         \
                                fog_b -= src_b;                         \
                        }                                               \
                                                                        \
                        switch (params->fogMode & (FOG_Z|FOG_ALPHA))    \
                        {                                               \
                                case 0:                                 \
                                fog_idx = (w_depth >> 10) & 0x3f;       \
                                                                        \
                                fog_a = params->fogTable[fog_idx].fog;  \
                                fog_a += (params->fogTable[fog_idx].dfog * ((w_depth >> 2) & 0xff)) >> 10;      \
                                break;                                  \
                                case FOG_Z:                             \
                                fog_a = (z >> 20) & 0xff;               \
                                break;                                  \
                                case FOG_ALPHA:                         \
                                fog_a = CLAMP(ia >> 12);                \
                                break;                                  \
                                case FOG_W:                             \
                                fog_a = CLAMP((w >> 32) & 0xff);        \
                                break;                                  \
                        }                                               \
                        fog_a++;                                        \
                                                                        \
                        fog_r = (fog_r * fog_a) >> 8;                   \
                        fog_g = (fog_g * fog_a) >> 8;                   \
                        fog_b = (fog_b * fog_a) >> 8;                   \
                                                                        \
                        if (params->fogMode & FOG_MULT)                 \
                        {                                               \
                                src_r = fog_r;                          \
                                src_g = fog_g;                          \
                                src_b = fog_b;                          \
                        }                                               \
                        else                                            \
                        {                                               \
                                src_r += fog_r;                         \
                                src_g += fog_g;                         \
                                src_b += fog_b;                         \
                        }                                               \
                }                                                       \
                                                                        \
                src_r = CLAMP(src_r);                                   \
                src_g = CLAMP(src_g);                                   \
                src_b = CLAMP(src_b);                                   \
        } while (0)

#define ALPHA_TEST(src_a)                               \
        do                                              \
        {                                               \
                switch (alpha_func)                     \
                {                                       \
                        case AFUNC_NEVER:               \
                        voodoo->fbiAFuncFail++;         \
                        goto skip_pixel;                \
                        case AFUNC_LESSTHAN:            \
                        if (!(src_a < a_ref))           \
                        {                               \
                                voodoo->fbiAFuncFail++; \
                                goto skip_pixel;        \
                        }                               \
                        break;                          \
                        case AFUNC_EQUAL:               \
                        if (!(src_a == a_ref))          \
                        {                               \
                                voodoo->fbiAFuncFail++; \
                                goto skip_pixel;        \
                        }                               \
                        break;                          \
                        case AFUNC_LESSTHANEQUAL:       \
                        if (!(src_a <= a_ref))          \
                        {                               \
                                voodoo->fbiAFuncFail++; \
                                goto skip_pixel;        \
                        }                               \
                        break;                          \
                        case AFUNC_GREATERTHAN:         \
                        if (!(src_a > a_ref))           \
                        {                               \
                                voodoo->fbiAFuncFail++; \
                                goto skip_pixel;        \
                        }                               \
                        break;                          \
                        case AFUNC_NOTEQUAL:            \
                        if (!(src_a != a_ref))          \
                        {                               \
                                voodoo->fbiAFuncFail++; \
                                goto skip_pixel;        \
                        }                               \
                        break;                          \
                        case AFUNC_GREATERTHANEQUAL:    \
                        if (!(src_a >= a_ref))          \
                        {                               \
                                voodoo->fbiAFuncFail++; \
                                goto skip_pixel;        \
                        }                               \
                        break;                          \
                        case AFUNC_ALWAYS:              \
                        break;                          \
                }                                       \
        } while (0)

#define ALPHA_BLEND(src_r, src_g, src_b, src_a)                         \
        do                                                              \
        {                                                               \
                int _a;                                                 \
                int newdest_r = 0, newdest_g = 0, newdest_b = 0;        \
                                                                        \
                switch (dest_afunc)                                     \
                {                                                       \
                        case AFUNC_AZERO:                               \
                        newdest_r = newdest_g = newdest_b = 0;          \
                        break;                                          \
                        case AFUNC_ASRC_ALPHA:                          \
                        newdest_r = (dest_r * src_a) / 255;             \
                        newdest_g = (dest_g * src_a) / 255;             \
                        newdest_b = (dest_b * src_a) / 255;             \
                        break;                                          \
                        case AFUNC_A_COLOR:                             \
                        newdest_r = (dest_r * src_r) / 255;             \
                        newdest_g = (dest_g * src_g) / 255;             \
                        newdest_b = (dest_b * src_b) / 255;             \
                        break;                                          \
                        case AFUNC_ADST_ALPHA:                          \
                        newdest_r = (dest_r * dest_a) / 255;            \
                        newdest_g = (dest_g * dest_a) / 255;            \
                        newdest_b = (dest_b * dest_a) / 255;            \
                        break;                                          \
                        case AFUNC_AONE:                                \
                        newdest_r = dest_r;                             \
                        newdest_g = dest_g;                             \
                        newdest_b = dest_b;                             \
                        break;                                          \
                        case AFUNC_AOMSRC_ALPHA:                        \
                        newdest_r = (dest_r * (255-src_a)) / 255;       \
                        newdest_g = (dest_g * (255-src_a)) / 255;       \
                        newdest_b = (dest_b * (255-src_a)) / 255;       \
                        break;                                          \
                        case AFUNC_AOM_COLOR:                           \
                        newdest_r = (dest_r * (255-src_r)) / 255;       \
                        newdest_g = (dest_g * (255-src_g)) / 255;       \
                        newdest_b = (dest_b * (255-src_b)) / 255;       \
                        break;                                          \
                        case AFUNC_AOMDST_ALPHA:                        \
                        newdest_r = (dest_r * (255-dest_a)) / 255;      \
                        newdest_g = (dest_g * (255-dest_a)) / 255;      \
                        newdest_b = (dest_b * (255-dest_a)) / 255;      \
                        break;                                          \
                        case AFUNC_ASATURATE:                           \
                        _a = MIN(src_a, 1-dest_a);                      \
                        newdest_r = (dest_r * _a) / 255;                \
                        newdest_g = (dest_g * _a) / 255;                \
                        newdest_b = (dest_b * _a) / 255;                \
                        break;                                          \
                }                                                       \
                                                                        \
                switch (src_afunc)                                      \
                {                                                       \
                        case AFUNC_AZERO:                               \
                        src_r = src_g = src_b = 0;                      \
                        break;                                          \
                        case AFUNC_ASRC_ALPHA:                          \
                        src_r = (src_r * src_a) / 255;                  \
                        src_g = (src_g * src_a) / 255;                  \
                        src_b = (src_b * src_a) / 255;                  \
                        break;                                          \
                        case AFUNC_A_COLOR:                             \
                        src_r = (src_r * dest_r) / 255;                 \
                        src_g = (src_g * dest_g) / 255;                 \
                        src_b = (src_b * dest_b) / 255;                 \
                        break;                                          \
                        case AFUNC_ADST_ALPHA:                          \
                        src_r = (src_r * dest_a) / 255;                 \
                        src_g = (src_g * dest_a) / 255;                 \
                        src_b = (src_b * dest_a) / 255;                 \
                        break;                                          \
                        case AFUNC_AONE:                                \
                        break;                                          \
                        case AFUNC_AOMSRC_ALPHA:                        \
                        src_r = (src_r * (255-src_a)) / 255;            \
                        src_g = (src_g * (255-src_a)) / 255;            \
                        src_b = (src_b * (255-src_a)) / 255;            \
                        break;                                          \
                        case AFUNC_AOM_COLOR:                           \
                        src_r = (src_r * (255-dest_r)) / 255;           \
                        src_g = (src_g * (255-dest_g)) / 255;           \
                        src_b = (src_b * (255-dest_b)) / 255;           \
                        break;                                          \
                        case AFUNC_AOMDST_ALPHA:                        \
                        src_r = (src_r * (255-dest_a)) / 255;           \
                        src_g = (src_g * (255-dest_a)) / 255;           \
                        src_b = (src_b * (255-dest_a)) / 255;           \
                        break;                                          \
                        case AFUNC_ACOLORBEFOREFOG:                     \
                        fatal("AFUNC_ACOLORBEFOREFOG\n"); \
                        break;                                          \
                }                                                       \
                                                                        \
                src_r += newdest_r;                                     \
                src_g += newdest_g;                                     \
                src_b += newdest_b;                                     \
                                                                        \
                src_r = CLAMP(src_r);                                   \
                src_g = CLAMP(src_g);                                   \
                src_b = CLAMP(src_b);                                   \
        } while(0)



void voodoo_queue_triangle(voodoo_t *voodoo, voodoo_params_t *params);

extern int voodoo_recomp;
extern int tris;
