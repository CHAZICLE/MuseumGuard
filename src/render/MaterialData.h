#ifndef __MATERIALDATA_H_INCLUDED__
#define __MATERIALDATA_H_INCLUDED__

#define MATERIAL_INDEX_Ka 0
#define MATERIAL_INDEX_Kd 1
#define MATERIAL_INDEX_Ks 2
#define MATERIAL_INDEX_Tf 3
#define MATERIAL_INDEX_d 4
#define MATERIAL_INDEX_Ns 5
#define MATERIAL_INDEX_Ni 6
#define MATERIAL_INDEX_illum 7
#define MATERIAL_INDEX_sharpness 8
#define MATERIAL_INDEX_map_Ka 9
#define MATERIAL_INDEX_map_Kd 10
#define MATERIAL_INDEX_map_Ks 11
#define MATERIAL_INDEX_map_Ns 12
#define MATERIAL_INDEX_map_d 13
#define MATERIAL_INDEX_disp 14
#define MATERIAL_INDEX_decal 15
#define MATERIAL_INDEX_bump 16

#define MATERIAL_MASK_Ka         (1<<MATERIAL_INDEX_Ka       )
#define MATERIAL_MASK_Kd         (1<<MATERIAL_INDEX_Kd       )
#define MATERIAL_MASK_Ks         (1<<MATERIAL_INDEX_Ks       )
#define MATERIAL_MASK_Tf         (1<<MATERIAL_INDEX_Tf       )
#define MATERIAL_MASK_d          (1<<MATERIAL_INDEX_d        )
#define MATERIAL_MASK_Ns         (1<<MATERIAL_INDEX_Ns       )
#define MATERIAL_MASK_Ni         (1<<MATERIAL_INDEX_Ni       )
#define MATERIAL_MASK_illum      (1<<MATERIAL_INDEX_illum    )
#define MATERIAL_MASK_sharpness  (1<<MATERIAL_INDEX_sharpness)
#define MATERIAL_MASK_map_Ka     (1<<MATERIAL_INDEX_map_Ka   )
#define MATERIAL_MASK_map_Kd     (1<<MATERIAL_INDEX_map_Kd   )
#define MATERIAL_MASK_map_Ks     (1<<MATERIAL_INDEX_map_Ks   )
#define MATERIAL_MASK_map_Ns     (1<<MATERIAL_INDEX_map_Ns   )
#define MATERIAL_MASK_map_d      (1<<MATERIAL_INDEX_map_d    )
#define MATERIAL_MASK_disp       (1<<MATERIAL_INDEX_disp     )
#define MATERIAL_MASK_decal      (1<<MATERIAL_INDEX_decal    )
#define MATERIAL_MASK_bump       (1<<MATERIAL_INDEX_bump     )

#endif
