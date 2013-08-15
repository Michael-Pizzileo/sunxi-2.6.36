#include "disp_display.h"
#include "disp_combined.h"
#include "disp_event.h"

__s32 BSP_disp_set_bk_color(__u32 sel, __disp_color_t *color)
{     
    if(color == NULL)
    {
        return DIS_PARA_FAILED;
    }
    
    gdisp.screen[sel].bk_color.blue=color->blue;
    gdisp.screen[sel].bk_color.red=color->red;
    gdisp.screen[sel].bk_color.green=color->green;

    DE_BE_Set_BkColor(sel, gdisp.screen[sel].bk_color);

    return DIS_SUCCESS;
}

__s32 BSP_disp_get_bk_color(__u32 sel, __disp_color_t *color)
{     
    if(color == NULL)
    {
        DE_WRN("para invalid in BSP_disp_get_bk_color\n");
        return DIS_PARA_FAILED;
    }
    color->blue = gdisp.screen[sel].bk_color.blue;
    color->red = gdisp.screen[sel].bk_color.red;
    color->green = gdisp.screen[sel].bk_color.green;

    return DIS_SUCCESS;
}


__s32 BSP_disp_set_color_key(__u32 sel, __disp_colorkey_t *ck_mode)
{
    if((ck_mode == NULL) || (ck_mode->red_match_rule > 3) || (ck_mode->green_match_rule > 3) || (ck_mode->blue_match_rule > 3))
    {
        DE_WRN("para invalid in BSP_disp_set_color_key\n");
        return DIS_PARA_FAILED;
    }
    memcpy(&(gdisp.screen[sel].color_key), ck_mode, sizeof(__disp_colorkey_t));
    DE_BE_Set_ColorKey(ck_mode->ck_max, ck_mode->ck_min, ck_mode->red_match_rule, ck_mode->green_match_rule, ck_mode->blue_match_rule);
   
   return DIS_SUCCESS;
}


__s32 BSP_disp_get_color_key(__u32 sel, __disp_colorkey_t *ck_mode)
{
   memcpy(ck_mode, &(gdisp.screen[sel].color_key),sizeof(__disp_colorkey_t));
   
   return DIS_SUCCESS;
}

__s32 BSP_disp_set_palette_table(__u32 sel, __u32 *pbuffer, __u32 offset, __u32 size)
{
    if((pbuffer == NULL) || (size < 0) || (offset < 0) || ((offset+size)>1024))
    {
        DE_WRN("para invalid in BSP_disp_set_palette_table\n");
        return DIS_FAIL;
    }
    DE_BE_Set_SystemPalette(pbuffer,offset, size);
    
    return DIS_SUCCESS;
}
      

__s32 BSP_disp_get_palette_table(__u32 sel, __u32 * pbuffer, __u32 offset,__u32 size)
{
    if((pbuffer == NULL) || (size < 0) || (offset < 0) || ((offset+size)>1024))
    {
        DE_WRN("para invalid in BSP_disp_get_palette_table\n");
        return DIS_FAIL;
    }

    DE_BE_Get_SystemPalette(pbuffer, offset,size);
    
    return DIS_SUCCESS;
}


__s32 BSP_disp_layer_set_top(__u32 sel, __u32  hid)
{     
    __s32 i,j;
    __u32 layer_prio[4];

    hid = HANDTOID(hid);
	HLID_ASSERT(hid, gdisp.screen[sel].max_layers);

    if(gdisp.screen[sel].layer_manage[hid].status & LAYER_USED)
    {
        __u32 prio = gdisp.screen[sel].max_layers-1;

        for(i=0; i<gdisp.screen[sel].max_layers; i++)
        {
            layer_prio[i] = gdisp.screen[sel].layer_manage[i].para.prio;
        }
        
        layer_prio[hid] = prio--;
        for(j=gdisp.screen[sel].max_layers-1; j>=0; j--)//for every prio from high to low
        {
            for(i=0; i<gdisp.screen[sel].max_layers; i++)//for every layer_prio that prio is j
            {
                if((gdisp.screen[sel].layer_manage[i].status & LAYER_USED) && (i != hid) && (gdisp.screen[sel].layer_manage[i].para.prio == j))
                {
                    layer_prio[i] = prio--;
                }
            }
        }

        for(i=0;i<gdisp.screen[sel].max_layers;i++)
    	{
    	   if(gdisp.screen[sel].layer_manage[i].status & LAYER_USED)
    	   {
    		  DE_BE_Layer_Set_Prio(sel, i, layer_prio[i]);
    		  gdisp.screen[sel].layer_manage[i].para.prio = layer_prio[i];
    	   }
    	}

    }
    else
    {
        return DIS_OBJ_NOT_INITED;
    }
    
    return DIS_SUCCESS;
}

__s32 BSP_disp_layer_set_bottom(__u32 sel, __u32  hid)
{     
    __s32 i,j;
    __u32 layer_prio[4];

    hid = HANDTOID(hid);
	HLID_ASSERT(hid, gdisp.screen[sel].max_layers);

    if(gdisp.screen[sel].layer_manage[hid].status & LAYER_USED)
    {
        __u32 prio = 0;
        
        for(i=0; i<gdisp.screen[sel].max_layers; i++)
        {
            layer_prio[i] = gdisp.screen[sel].layer_manage[i].para.prio;
        }

        layer_prio[hid] = prio++;
        for(j=0; j<gdisp.screen[sel].max_layers; j++)//for every prio from low to high
        {
            for(i=0; i<gdisp.screen[sel].max_layers; i++)//for every layer that prio is j
            {
                if((gdisp.screen[sel].layer_manage[i].status & LAYER_USED) && (i != hid) && (gdisp.screen[sel].layer_manage[i].para.prio == j))
                {
                    layer_prio[i] = prio++;
                }
            }
        }

        for(i=0;i<gdisp.screen[sel].max_layers;i++)
    	{
    	   if(gdisp.screen[sel].layer_manage[i].status & LAYER_USED)
    	   {
    		  DE_BE_Layer_Set_Prio(sel, i, layer_prio[i]);
    		  gdisp.screen[sel].layer_manage[i].para.prio = layer_prio[i];
    	   }
    	}

    }
    else
    {
        return DIS_OBJ_NOT_INITED;
    }
    
    return DIS_SUCCESS;
}

__s32 BSP_disp_layer_set_alpha_value(__u32 sel, __u32 hid,__u8 value)
{
    hid = HANDTOID(hid);
    HLID_ASSERT(hid, gdisp.screen[sel].max_layers);

    if(gdisp.screen[sel].layer_manage[hid].status & LAYER_USED)
    {
        DE_BE_Layer_Set_Alpha_Value(sel, hid, value);

        gdisp.screen[sel].layer_manage[hid].para.alpha_val = value;
    }
    else
    {
        return DIS_OBJ_NOT_INITED;
    }

    return DIS_SUCCESS;
}

      

__s32 BSP_disp_layer_get_alpha_value(__u32 sel, __u32 hid)
{
    hid = HANDTOID(hid);
    HLID_ASSERT(hid, gdisp.screen[sel].max_layers);

    if(gdisp.screen[sel].layer_manage[hid].status & LAYER_USED)
    {
        return gdisp.screen[sel].layer_manage[hid].para.alpha_val;
    }
    else
    {
        return DIS_OBJ_NOT_INITED;
    }
}
      

__s32 BSP_disp_layer_alpha_enable(__u32 sel, __u32 hid, __bool enable)
{    
    hid = HANDTOID(hid);
    HLID_ASSERT(hid, gdisp.screen[sel].max_layers);

    if(gdisp.screen[sel].layer_manage[hid].status & LAYER_USED)
    {
        DE_BE_Layer_Alpha_Enable(sel, hid, enable);

        gdisp.screen[sel].layer_manage[hid].para.alpha_en = enable;
    }
    else
    {
        return DIS_OBJ_NOT_INITED;
    }

    return DIS_SUCCESS;
}

__s32 BSP_disp_layer_get_alpha_enable(__u32 sel, __u32 hid)
{
    hid = HANDTOID(hid);
    HLID_ASSERT(hid, gdisp.screen[sel].max_layers);

    if(gdisp.screen[sel].layer_manage[hid].status & LAYER_USED)
    {
        return gdisp.screen[sel].layer_manage[hid].para.alpha_en;
    }
    else
    {
        return DIS_OBJ_NOT_INITED;
    }
}

__s32 BSP_disp_layer_set_pipe(__u32 sel, __u32 hid,__u8 pipe)
{    
    hid = HANDTOID(hid);
    HLID_ASSERT(hid, gdisp.screen[sel].max_layers);
    if(pipe != 0 && pipe != 1)
    {
        return DIS_OBJ_NOT_INITED;
    }

    if(gdisp.screen[sel].layer_manage[hid].status & LAYER_USED)
    {
        DE_BE_Layer_Set_Pipe(sel, hid,pipe);

        gdisp.screen[sel].layer_manage[hid].para.pipe= pipe;
    }
    else
    {
        return DIS_OBJ_NOT_INITED;
    }

    return DIS_SUCCESS;
}
     

__s32 BSP_disp_layer_get_pipe(__u32 sel, __u32 hid)
{
    hid = HANDTOID(hid);
    HLID_ASSERT(hid, gdisp.screen[sel].max_layers);

    if(gdisp.screen[sel].layer_manage[hid].status & LAYER_USED)
    {
        return gdisp.screen[sel].layer_manage[hid].para.pipe;
    }
    else
    {
        return DIS_OBJ_NOT_INITED;
    }
}
      

__s32 BSP_disp_layer_colorkey_enable(__u32 sel, __u32 hid, __bool enable)
{    
    hid = HANDTOID(hid);
    HLID_ASSERT(hid, gdisp.screen[sel].max_layers);

    if(gdisp.screen[sel].layer_manage[hid].status & LAYER_USED)
    {
        DE_BE_Layer_ColorKey_Enable(sel, hid,enable);

        gdisp.screen[sel].layer_manage[hid].para.ck_enable = enable;
    }
    else
    {
        return DIS_OBJ_NOT_INITED;
    }

    return DIS_SUCCESS;
}

__s32 BSP_disp_layer_get_colorkey_enable(__u32 sel, __u32 hid)
{
    hid = HANDTOID(hid);
    HLID_ASSERT(hid, gdisp.screen[sel].max_layers);

    if(gdisp.screen[sel].layer_manage[hid].status & LAYER_USED)
    {
        return gdisp.screen[sel].layer_manage[hid].para.ck_enable;
    }
    else
    {
        return DIS_OBJ_NOT_INITED;
    }
}

__s32 BSP_disp_layer_get_piro(__u32 sel, __u32 hid)
{
    hid = HANDTOID(hid);
    HLID_ASSERT(hid, gdisp.screen[sel].max_layers);

    if(gdisp.screen[sel].layer_manage[hid].status & LAYER_USED)
    {
        return gdisp.screen[sel].layer_manage[hid].para.prio;
    }
    else
    {
        return DIS_OBJ_NOT_INITED;
    }
}
      

      
