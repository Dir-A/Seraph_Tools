## Structures

### File Structures

```c
struct Seraph_Dat_Entry
{
	uint32_t uiDataFOA;
    // data_size = uiDataFOA - next_entry_FOA
};

struct Seraph_Dat_Data
{
	uint32_t uiCompressFlag; //zlib
	uint8_t aData[data_size];
};

struct Seraph_Dat_Segment_File_Entry
{
    uint32_t uiDataOffsetInSeg; // -> FOA = SegmengtOffset + uiSeraphDataOffsetInSeg
    // data_size = uiSeraphDataFOA - next_entry_Offset
};

struct Seraph_Dat_Segment_Entry
{
    uint32_t uiSegmentFOA;
    uint32_t uiFileCount;
};

struct Seraph_Dat_Sengmet_HDR
{
    uint32_t uiSegmentCount;
    uint32_t uiFileCount;
};

struct Seraph_Dat_Type0
{
	uint32_t uiFileCount;
	Seraph_Dat_Entry aEntryList[uiFileCount + 1];// last entry is pack_file_size - 1
	Seraph_Dat_Data aDataList[uiFileCount]
};

struct Seraph_Dat_Type1
{
    Seraph_Dat_Sengmet_HDR HDR; //<- 雛鳥の堕ちる音: ArchPac.dat: 0x05EBB40D
    Seraph_Dat_Segment_Entry aSegEntry[HDR.uiSegmentCount];
    Seraph_Dat_Segment_File_Entry aSegFilesIndex[aSegEntry[-1].uiFileCount +1];
    Seraph_Dat_Segment_File_Entry aSegFilesIndex[aSegEntry[-2].uiFileCount + 1];
    Seraph_Dat_Segment_File_Entry aSegFilesIndex[aSegEntry[-3].uiFileCount + 1];
    ...;
    Seraph_Dat_Segment_File_Entry aSegFilesIndex[aSegEntry[0].uiFileCount + 1]
};
```

### Memory Structures

```c
Seraph_Arch_Entry* sg_ArchIndexPtr = 雛鳥の堕ちる音: 0x0045EF9C;

struct Seraph_Arch_Entry
{
  uint32_t uiIsCash;
  Seraph_Dat_Entry Entry;
};

struct Seraph_Dat_Entry
{
  uint32_t uiDataFOA;
  uint32_t uiDataSize;
};


struct Seraph_Music_FileName_Table_Entry
{
    uint8_t ucSeq;
    char aFileName[??]; //with null char
    uint8_t ucFlag;
   	if ucFlag
        
    else
        uint16_t usUn;
};

struct Seraph_Music_FileName_Table
{
    Seraph_Music_FileName_Table_Entry aIndex[??]
}
```

## Command

Based on [2006] [雛鳥の堕ちる音](https://vndb.org/v25450) 

### ExpressionCommand

```c
// 0x00 
struct get_stack_0
{
    [BYTE:op]
    [BYTE:seq]
};

// 0x01 
struct check_stack_1
{
    [BYTE:op]
    [BYTE:type]
    if{type==1}
    {
        [EXP:seq]
    }
    else
    {
        [WORD:seq]
        [BYTE:un]
    }
};

// 0x02 
struct get_stack_2_type0
{
    [BYTE:op]
    [WORD:seq]
};

// 0x03 
struct get_stack_2_type1
{
    [BYTE:op]
    [WORD:seq]
    [EXP:un]
};

// 0x04
struct get_stack_2_type2
{
    [BYTE:op]
    [WORD:un]
    [WORD:un]
    [EXP:un]
    [WORD:un]
    [EXP:un]
};
    
// 0x05
struct get_dword
{
    [BYTE:op]
    [DWORD:val]
};

// 0x06
struct read_res
{
    [BYTE:op]
    [WORD:res_table_seq]
    [WORD:size]
    [EXP:size]
    [BYTE:read_type]
    [WORD:res_base]
    [WORD:type]
};

// 0x07 
struct get_random_val
{
    [BYTE:op]
    [DWORD:retain]
};

// 0x10
struct check_status
{
    [BYTE:op]
    if{status == true} 
    {
        pc += 5
    }
};

0x08 [+]
0x09 [-]
0x0A [*]
0x0B [/]
0x0C [%]
0x0D [&]
0x0E [|]
0x0F [^]
0x11 [==]
0x12 [!=]
0x13 [<=]
0x14 [>=]
0x15 [<]
0x16 [>]
0x17 [&&]
0x18 [||]

// 0x19
struct set_pc
{
    [BYTE:op]
    [DWORD:pc]
};
```



### StackCommand

```c
```



### SystemCommand

```c
0x00 [stack_command]
{
    [BYTE:op]
    [StackCommand]
};

0x01 [go 0x04]
0x02 [nop]
0x03 [go 0x04]

0x04 [jz]
{
    [BYTE:op]
    [EXP:un]
}; 

0x05 [set_flag]
{
    [BYTE:op]
    [EXP:un]
}; 

0x06 [check_flag/jne]
{
    [BYTE:op]
    [EXP:un]
    [DWORD:new_pc] // if check_flag != set_flag -> set pc = new_pc
};

0x07 [nop]
0x08 [nop]
0x09 [nop]

0x0A [set_pc/jmp]
{
    [BYTE:op]
    [DWORD:new_pc]
};

0x0B [jmp xx]
{
    [BYTE:op]
    [DWORD:new_pc]
};

0x0C [jmp xx]
{
    [BYTE:op]
};

0x0D [jmp xx]
{
    [BYTE:op]
    [EXP:un]
    [WORD:un]
};

0x0E [unknow]
{
    [BYTE:op]
    [EXP:un]
    [DWORD:un]
};

0x0F [call script2]
{
    [BYTE:op]
    [DWORD:new_pc] // jmp to script2 ,set pc to new_pc
};

0x10 [ret script2]
{
    [BYTE:op]
};

0x11 [go 0xFF]

0x12 [unknow]
{
    [BYTE:op]
    [EXP:un]
    [DWORD:un]
};

0x13 [jmp_delay]
{
    [BYTE:op]
    [DWORD:pc_back]
    [EXP:un]
};

0x14 [ret xxx]
{
    [BYTE:op]
    [BYTE:end_flag] 
};

0x15 [set_system_commnad_end_flag]
{
    [BYTE:op]
    [BYTE:end_flag] 
};

0x16 [nop]
{
    [BYTE:op]
};

0x17 [nop]
{
    [BYTE:op]
};

0x18 [arch_open]
{
    [BYTE:op]
    [EXP:arch_index_table_offset]
};

0x19 [set_cash_flag]
{
    [BYTE:op]
    [BYTE:cash_flag]
};

0x1A [zero_un_obj]
{
    [BYTE:op]
};

0x1B [unknow]
{
    [BYTE:op]
    if flag = -1
        [EXP:un]
     else
         [WORD:un][BYTE:flag]
     [BYTE:flag]
};

0x1C [music_index_read]
{
    [BYTE:op]
    [Seraph_Music_FileName_Table]
    [BYTE:end_flag=0xFF]
};

0x1D [music_index2_read]
{
    [BYTE:op]
    [Seraph_Music_FileName_Table]
    [BYTE:end_flag=0xFF]
};

0x1E [xxx_index_read]
{
    [BYTE:op]
    ?????
};

0x1F [set_packstream_param] 
{
    [BYTE:op]
    [BYTE:mod]
    [WORD:seq]
    [EXP:seq]
};

0x20 [xxx_config_flag_read]
{
    [BYTE:op]
    [WORD:un_flag]
};

0x28 [enable_menu_skip]
{
    [BYTE:op]
};

0x47 [switch_to_scenario_command_func]
{
    [BYTE:op]
};

0x48 [font_color_set]
{
    [BYTE:op]
    [EXP:color_r]
    [EXP:color_g]
    [EXP:color_b]
};

0x49 [font_rect_set]
{
    [BYTE:op]
    [EXP:width]
    [EXP:heigh]
};

0x4A [unknow_set]
{
    [BYTE:op]
    [EXP:un]
};

0x4B [unknow_set]
{
    [BYTE:op]
    [EXP:un]
};

0x56 [set_select]
{
    [BYTE:op]
    [EXP:un]
    [EXP:un]
    [EXP:un]
    [EXP:un]
    [EXP:un]
    [val...]
    [text_0]
    [text_1]
};

0x57 [get_pressed_key]
{
    [BYTE:op]
};

0x5B [get_click_rect]
{
    [BYTE:op]
};

0x5C [get_click_rect]
{
    [BYTE:op]
};

0x60 [load_imge]
{
    [BYTE:op]
    [BYTE:pack_stream_mod]
    if pack_stream_mod != 0xFF
        [word:pack_stream_seq]
    [EXP:un]
    [EXP:un]
    [EXP:un]
    [EXP:un]
    [BYTE:flag]
    if flag == 0xFF
        [EXP:un]
        [EXP:un]
};

0x71 [load_un]
{
    [BYTE:op]
    [byte:ps_mod]
    if ps_mod == 0xFE
        [word:ps_seq][EXP:un]
    else
        [word:ps_seq][EXP:un]
};
    
0x74 [unknow]
{
    [BYTE:op]
};

0X9C [cls_background]
{
    [BYTE:op]
};

0x9E [copy_imge]
{
    [BYTE:op]
    [EXP:un]
};

0x9F [music_open]
{
    [BYTE:op]
    [EXP:un]
};

0xA0 [music_play]
{
    [BYTE:op]
    [BYTE:un]
};

0xA1 [music_stop]
{
    [BYTE:op]
};

0xA2 [music_close]
{
    [BYTE:op]
};

0xA6 [music_play_]
{
    [BYTE:op]
};

0xA9 [mod_switch_to]
{
    [BYTE:op]
};

0xAA [music_cd_drive_play]
{
    [BYTE:op]
};

0xAB [music_cd_drive_stop]
{
    [BYTE:op]
};

0xAE [??]
{
    [BYTE:op]
    [EXP:un]
};

0xB4 [script_load]
{
    [BYTE:op]
    [pack_stream_param]
    []
};

0xB8 [sav_save]
{
    [BYTE:op]
    [EXP:un]
};

0xB9 [sav_load]
{
    [BYTE:op]
    [EXP:un]
};

0xBA [sav_enum]
{
    [BYTE:op]
    [EXP:un]
};

0xBB [env_write]
{
    [BYTE:op]
};

0xBC [env_read]
{
    [BYTE:op]
};

0xBE [thumbnail_read_info]
{
    [BYTE:op]
    [EXP:thumbnail_width]
    [EXP:thumbnail_heigh]
};

0xBF [thumbnail_create]
{
    [BYTE:op]
};

0xC0 [thumbnail_draw]
{
    [BYTE:op]
    [EXP:thumbnail_seq_in_sav_file]
    [EXP:screen_x]
    [EXP:screen_y]
};
    
0xC1 [thumbnail_convert]
{
    [BYTE:op]
}; 

0xD5 [bmp_load]
{
    [BYTE:op]
};

0xFF [read_dword_but_not_used]
{
    [BYTE:op]
    [DWORD:value]
};
```

### ScenarioCommand

```
scenario command begin FF0F091F000047
```

```c
//0x00
struct text_push
{
    [BYTE:op]
    char aMsg[??] // with null char
};
    
// 0x01
struct text_format
{
    [BYTE:op]
    [EXP:un]
};

// 0x02
struct font_set_color_r
{
    [BYTE:op]
    [BYTE:color_r]
};

// 0x03
struct font_set_color_g
{
    [BYTE:op]
    [BYTE:color_g]
};

// 0x04
struct font_set_color_b
{
    [BYTE:op]
    [BYTE:color_b]
};

// 0x05
struct font_set_width
{
    [BYTE:op]
    [BYTE:font_width]
};

// 0x06 
struct font_set_heigh
{
    [BYTE:op]
    [BYTE:font_heigh]
};

// 0x07 
struct not_used
{
    [BYTE:op]
    [BYTE:un]
};

// 0x08
// 08 0B -> wait (0xB * 10) ms
struct time_wait
{
    [BYTE:op]
    [BYTE:time] 
};

// 0x09 
struct text_allow_color_green
{
    [BYTE:op]
    [BYTE:un] 
};

// 0x0A
struct text_draw_layer 
{
    [BYTE:op]
    [BYTE:layer] 
};

// 0x0B 
struct text_allow_vertical
{
    [BYTE:op]
    [BYTE:un] 
};

// 0x0E 
struct text_indent_add
{
    [BYTE:op]
    [BYTE:len] // pixel = len * 8 
};

// 0x0F 
struct se_play
{
    [BYTE:op]
    [BYTE:sound_seq] 
};

// 0x10
// 逐字显示文本的过程中是否允许点击略过文本显示动画
struct cursor_allow_click
{
    [BYTE:op]
    [BYTE:uj] 
};

// 0x11 
struct text_indent_at
{
    [BYTE:op]
    [BYTE:pos] 
};

// 0x14 
struct next_line
{
    [BYTE:op]
};

// 0x15
// render text and wait input
struct input_wait
{
    [BYTE:op]
};


// 0x16 
struct text_indent_flag
{
    [BYTE:op]
};

// 0x17 
struct next_page
{
    [BYTE:op]
};

// 0x18 
struct voice_play
{
    [BYTE:op]
    [DWORD:voice_file_seq]
};

// 0x19 
struct screen_shake
{
    [BYTE:op]
};

// 0xFF
// end of code block. back to SystemCommand parse mode
struct end
{
    [BYTE:op]
};
```

#### Human Readable

```asm
0x00 
0x01 text_format            -> format   -> [format=exp]
0x02 font_set_color_r       -> fr       -> [fr=10]
0x03 font_set_color_g       -> fg       -> [fg=10]
0x04 font_set_color_b       -> fb       -> [fb=10]
0x05 font_set_width         -> fw       -> [fw=10]
0x06 font_set_heigh         -> fh       -> [fh=10]
0x07 
0x08 time_wait              -> w        -> [w=10]
0x09 text_allow_color_green -> tag      -> [tag=10]
0x0A text_draw_layer -> tdl -> layer    -> [layer=10]
0x0B text_allow_vertical    -> vertical -> [vertical=10]
0x0C 
0x0D
0x0E text_indent_add        -> tia      -> [tia=10]
0x0F se_play                -> se       -> [se=10]
0x10 cursor_allow_click     -> click    -> [click=10]
0x11 text_indent_at         -> tit      -> [tit=10]
0x12
0x13
0x14 next_line              -> n        -> [n]
0x15 input_wait             -> r        -> [r]
0x16 text_indent_flag       -> t        -> [t]
0x17 next_page              -> p        -> [p]
0x18 voice_play             -> voice    -> [voice=10]
0x19 screen_shake           -> shake    -> [shake]
0xFF end                    -> end      -> [end]
```



## Method of text replacement

这玩意又是文本夹在代码中间的，分析了vm发现内部有相对或绝对的跳转，肯定是不能直接变换文本长度了。不过找到个SystemCommand的op:0x0A，是直接跳转的指令，也就是直接设置pc，所以理论上也可以使用和[Valkyria](https://github.com/Dir-A/Valkyria_Tools)差不多的思路，即，用vm的jmp指令跳出来，然后改文本再跳回去，有点类似于inline hook的样子，只不过这个是利用vm自己的指令，而且这个引擎的文本控制还是单独指令集的，即，SystemCommand有个指令可以把当前vm解析的指令模式切换到文本控制的指令集ScenarioCommand，而且这个指令集指令很少，基本分析完了，所以只要扫描SystemCommand切换到ScenarioCommand的指令，定位到ScenarioCommand的代码块（切换指令其后就是ScenarioCommand的代码块,扫描的特征码FF0F091F000047，最后这个47就是切换模式的op，同时ScenarioCommand的代码块以0xFF指令退出，也就是代码块的结束），然后写个jmp指令跳到文件末尾，解析ScenarioCommand的代码块就可以随便插入文本了，在代码块结束的位置跳回去就好了。主要是因为文本控制的op很少，分析省时间，完整的op很多，分析要太久，熬不住。

不过要注意的是，这样是整个代码块的替换，但会不会有指令往代码块中间跳？很难讲，不过往中间跳其实也没什么问题，就是会造成文本变回原来的，真有的话，其实也可以很轻松定位到跳转的指令。那往代码块开头跳的情况呢？理论上不会，如果它全部进入ScenarioCommand的代码块的op都是用47的话，当然要是真有的话，我们就要注意不要去覆盖代码块的内容，特别是开头的地方，至少保证引擎不要崩溃，不然调试会很麻烦。



## Bypass CD Check

### Command Line Arguments Check

`0x00412BB3`

游戏启动的时候会传递两个参数
从它安装后创建的图标里可以看到参数设置

```asm
C:\Hinadori\Seraph.exe Hinadori 雛鳥の堕ちる音
```

第一个参数是游戏的英文名或者说罗马音表示，第二个是日文名

两个字符串进入WinMain后会被sscanf读取到两个全局的变量中，英文名主要用于查找游戏的配置INI文件，日文名主要用于设置游戏标题

如果启动缺少这两个参数，会在sscnf的返回值检测出来，从而退出程序

### INI File Check

`0x00412C00`

紧接着游戏通过 GetPrivateProfileStringA 来从 Assemblage.ini 读取在游戏英文名为section的ini中的key - val，由于 GetPrivateProfileStringA 的设置，不传递完整路径从 Windows 目录读取，所以游戏安装的时候会把 Assemblage.ini 写到Windows目录里，如果Windows目录里没有该文件，或者不存在section中的InstDIR键值则程序退出

### CD Drive Check

`0x00412CE1`

继续往下来到一个cd检测函数，该函数内部主要调用 `0x004129F0` 这个函数，这个函数会循环枚举 a b c d e ... 等盘符来检查盘符名是否为游戏启动时输入的英文名参数，如果检测失败则会提示重试



## Seraph Engine Games

|    Date    |               Developer                |                           Title                           | Type | ArchPac Index Offset | Version |
| :--------: | :------------------------------------: | :-------------------------------------------------------: | :--: | :------------------: | :-----: |
| 2009-08-16 | [Digital Lover](https://vndb.org/p881) | [School Captain 会長候補はご立腹](https://vndb.org/v2476) | PKG  |     `0x08FC6E04`     |   V3    |
| 2009-12-31 | [Digital Lover](https://vndb.org/p881) | [School Captain 2 会王をねらえ!](https://vndb.org/v3212)  | PKG  |     `0x02AFD433`     |   V3    |
| 2010-08-15 | [Digital Lover](https://vndb.org/p881) |         [制服楽園EXTEND](https://vndb.org/v5053)          | PKG  |     `0x0523338A`     |   V3    |
| 2006-12-31 |     [Ex12](https://vndb.org/p1208)     |         [雛鳥の堕ちる音](https://vndb.org/v25450)         | PKG  |     `0x05EBB40D`     |   V2    |
| 2000-07-14 |    [Ather](https://vndb.org/p1041)     |          [好きだよっ！](https://vndb.org/v4923)           | PKG  |                      |   V1    |
|            |                                        |                                                           |      |                      |         |



## Character Name Display

```c
struct param
{
   [BYTE:op]
    
   [BYTE:type]
   [val]
   [BYTE:type]
   [val]
   [BYTE:type]
   [val]
   ...
       
   [BYTE:end=0xFF]
}；

// type draw text box with char name
2C                save pc
00 02 0B00 FF     push param0
00 05 00000000 FF push param1
00 00 10 FF       push param2
00 05 0B000000 FF push param3       -> char_name_image_file_seq_offset
0F 091F0000       jmp script 2 to offset 0x91F (call draw text box)
47                switch to scenario command parse mod

// type draw text box without char name
2C
00 00 10 FF
00 05 00000000 FF
0F 091F0000 
47

00[op] 02[type] 0B00[val] FF[end]     push param0

00 05 0B000000 FF 0F 091F0000 47
______^____________________________ char_name_image_file_seq_offset

char_name_image_file_seq_base = 0xE9 <-雛鳥の堕ちる音
char_name_image_file_seq_offset
char_name_image_file_seq = char_name_image_file_seq_base + char_name_image_file_seq_offset
```

## Select Box

```asm
56                         -> op
05 03000000 FF             -> exp
05 00000000 FF             -> exp
05 00000000 FF             -> exp
05 00000000 FF             -> exp
05 00000000 FF             -> exp
7C3F0000                   -> pc
FE                         -> if == 0xFF end code
02                         -> select text count
7B3F0000                   -> pc
8DF782A982E78F5082A400     -> "桜から襲う"
93FA8CFC82A982E78F5082A400 -> "日向から襲う"
FF                         -> end


56
0503000000FF
0500000000FF
0500000000FF
0500000000FF
0500000000FF
58480000
FE
02
57480000
E45393E082C98F6F82B700   -> "膣内に出す"
82D482C182A982AF82E900   -> "ぶっかける"
FF
```

