//############################################
// bmp to header file convertor version 3.0
// Author : www.ThaiEasyElec.com
//############################################
// source bmp file : D:\Project\Farasn\Arduino\RedBtn.bmp
// source pixel dept : 32
// target name : D:\Project\Farasn\Arduino\RedBtn.h
// target pixel dept : 16
// target width : 89
// target height : 50
// ############################################
const unsigned int  RedLED[] = {};
/*
{
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xDFBF,0x973F,0x5EFF,0x369F,0x265F,0x263F,0x1E3F,
	0x1E1F,0x1E1F,0x1DFF,0x1DFF,0x1DDF,0x1DDF,0x1DBF,0x1D9F,
	0x1D7E,0x1D5F,0x1D3E,0x1D1F,0x1CFE,0x1CDE,0x1C9E,0x149E,
	0x149E,0x145E,0x143E,0x141E,0x13DE,0x13FE,0x143E,0x145E,
	0x147E,0x14BE,0x14DF,0x14FE,0x151E,0x151E,0x153E,0x155E,
	0x1D7E,0x1D9E,0x1DBE,0x1DDE,0x1DFF,0x1E1E,0x1E3E,0x1E5F,
	0x1E7E,0x269F,0x26BF,0x36DE,0x46FE,0x5F3E,0x9F9F,0xE7DF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xDFBF,
	0x66DF,0x063F,0x061F,0x061F,0x05FF,0x05DF,0x05DF,0x05DF,
	0x05BF,0x059F,0x057F,0x055F,0x053F,0x053F,0x051F,0x04FF,
	0x04BF,0x049F,0x045F,0x041F,0x03DF,0x03BF,0x035F,0x033F,
	0x037F,0x03BF,0x03FF,0x043F,0x045F,0x049F,0x04BF,0x04DF,
	0x04DF,0x04FF,0x04FF,0x051F,0x053F,0x055F,0x057F,0x059F,
	0x05BF,0x05DF,0x05FF,0x061F,0x063F,0x065F,0x067F,0x069F,
	0x06BF,0x06DF,0x0EFF,0x2F1F,0x673F,0x879F,0xAF9F,0xB79F,
	0xA79F,0xA79F,0xDFDF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xCF9F,0x365E,
	0x05FF,0x063F,0x063F,0x0E3F,0x061F,0x061F,0x05FF,0x05DF,
	0x05BF,0x05BF,0x059F,0x057F,0x055F,0x053F,0x051F,0x051F,
	0x04FF,0x04DF,0x04BF,0x047F,0x043F,0x03FF,0x03DF,0x037F,
	0x033F,0x035F,0x039F,0x03DF,0x041F,0x043F,0x047F,0x049F,
	0x04BF,0x04DF,0x04FF,0x04FF,0x051F,0x053F,0x055F,0x057F,
	0x059F,0x05BF,0x05DF,0x05DF,0x061F,0x0E1F,0x065F,0x0E5F,
	0x0E9F,0x0E9F,0x0EBF,0x06DF,0x1F1F,0x4F3F,0x7F7F,0xA79F,
	0xD7DF,0xFFFF,0xE7FF,0xC7BF,0xD7BF,0xEFFF,0xF7FF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x4E3E,0x05BF,
	0x063F,0x063F,0x065F,0x0E3F,0x063F,0x061F,0x061F,0x165F,
	0x2E9F,0x46DF,0x5EDF,0x66DF,0x56BF,0x469F,0x2E7F,0x165F,
	0x061F,0x05FF,0x05DF,0x05BF,0x057F,0x055F,0x051F,0x04FF,
	0x04BF,0x045F,0x045F,0x047F,0x049F,0x049F,0x04BF,0x04DF,
	0x04FF,0x04FF,0x04FF,0x04FF,0x04FF,0x04DF,0x04DF,0x04DF,
	0x04DF,0x051F,0x053F,0x057F,0x059F,0x05DF,0x05FF,0x061F,
	0x063F,0x065F,0x067F,0x069F,0x06BF,0x0EDF,0x2F1F,0x5F5F,
	0x979F,0xCFDF,0xFFFF,0xF7FF,0xD7BF,0xB7BF,0x9F9F,0xAF9F,
	0xD7BF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xBF1F,0x053E,0x061F,
	0x065F,0x0E5F,0x0E3F,0x0E5F,0x0E5F,0x26BF,0x573F,0x773F,
	0x7E5B,0x7D37,0x8495,0x8C94,0x9CB5,0x8495,0x5C75,0x4435,
	0x2415,0x13F5,0x13F5,0x13D5,0x13B5,0x13B5,0x1395,0x1395,
	0x0B75,0x0B75,0x0B55,0x0B35,0x0B35,0x0B15,0x0AF5,0x0AD5,
	0x0AD5,0x0AD4,0x0AB5,0x0A95,0x0A75,0x0A55,0x0A35,0x0215,
	0x01D5,0x09D5,0x09F5,0x0A35,0x0A55,0x0A75,0x0A95,0x0AB5,
	0x0AD5,0x0AD5,0x0AF5,0x0AF5,0x0B15,0x0B15,0x0B35,0x0BD7,
	0x0CDB,0x0E1F,0x371F,0x879F,0xEFFF,0xEFFF,0xC7BF,0x9F9F,
	0x877F,0x773F,0x977F,0xDFDF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x85BE,0x04DF,0x065F,
	0x0E3F,0x0E3F,0x065F,0x0E5F,0x1E9F,0x3DBE,0x2B33,0x08CA,
	0x0005,0x0005,0x0005,0x0005,0x0005,0x0005,0x0006,0x0006,
	0x0005,0x0005,0x0005,0x0006,0x0005,0x0006,0x0006,0x0006,
	0x0006,0x0006,0x0006,0x0006,0x0006,0x0006,0x0005,0x0005,
	0x0006,0x0006,0x0006,0x0005,0x0005,0x0006,0x0005,0x0005,
	0x0005,0x0006,0x0006,0x0006,0x0005,0x0005,0x0005,0x0005,
	0x0006,0x0005,0x0005,0x0005,0x0005,0x0006,0x0006,0x0005,
	0x0005,0x0005,0x0005,0x008A,0x02F3,0x0D9D,0x4F5F,0xAFDF,
	0xBFBF,0x977F,0x7F3F,0x66DF,0x671F,0xBF9F,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x5D3E,0x04DF,0x0E7F,
	0x065F,0x0E5F,0x065F,0x0E3F,0x1CFC,0x114D,0x0004,0x0006,
	0x0007,0x0009,0x000C,0x000D,0x000E,0x000D,0x000D,0x000D,
	0x000D,0x000E,0x000E,0x000E,0x000E,0x000C,0x000D,0x000C,
	0x000E,0x000E,0x000E,0x000E,0x000C,0x000D,0x000C,0x000E,
	0x000E,0x000E,0x000E,0x000C,0x000D,0x000D,0x000D,0x000E,
	0x000E,0x000E,0x000D,0x000D,0x000D,0x000D,0x000E,0x000E,
	0x000E,0x000D,0x000D,0x000D,0x000D,0x000E,0x000E,0x000E,
	0x000D,0x000C,0x000B,0x0009,0x0008,0x0005,0x0004,0x014D,
	0x055C,0x475F,0xA79F,0x877F,0x6F1F,0x569F,0x56BF,0xA75F,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x655E,0x04FF,0x067F,
	0x0E5F,0x065F,0x0E5F,0x0DBF,0x09AF,0x0005,0x0006,0x000A,
	0x000C,0x000D,0x000E,0x000F,0x000F,0x000F,0x000F,0x000E,
	0x000E,0x000D,0x000F,0x000F,0x000F,0x000F,0x000E,0x000E,
	0x000D,0x000F,0x0010,0x000F,0x000F,0x000E,0x000E,0x000E,
	0x000F,0x000F,0x000F,0x000F,0x000E,0x000E,0x000E,0x000F,
	0x000F,0x000F,0x000F,0x000E,0x000E,0x000E,0x000F,0x0010,
	0x000F,0x000F,0x000E,0x000E,0x000E,0x000E,0x000F,0x000F,
	0x000F,0x000E,0x000E,0x000E,0x000E,0x000F,0x000E,0x000A,
	0x0006,0x0004,0x01F0,0x169F,0x6F7F,0x7F5F,0x66DF,0x4E7F,
	0x3E7F,0x9F3F,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x8E1E,0x049F,0x0E7F,
	0x0E5F,0x0E5F,0x065F,0x0CBC,0x0007,0x0005,0x0009,0x000F,
	0x000E,0x000D,0x000E,0x000E,0x000F,0x000F,0x000F,0x000F,
	0x000E,0x000E,0x000E,0x000F,0x0010,0x0010,0x000F,0x000E,
	0x000D,0x000D,0x000F,0x000F,0x000F,0x000F,0x000D,0x000E,
	0x000D,0x000F,0x000F,0x000F,0x000F,0x000E,0x000D,0x000D,
	0x000F,0x000F,0x000F,0x000F,0x000D,0x000D,0x000D,0x000E,
	0x000F,0x000F,0x000F,0x000E,0x000E,0x000D,0x000E,0x0010,
	0x000F,0x000F,0x000E,0x000E,0x000E,0x000E,0x0010,0x000F,
	0x000F,0x000D,0x0009,0x0005,0x0007,0x0D7C,0x475F,0x771F,
	0x56BF,0x3E3F,0x363F,0xB75F,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xCF1F,0x041E,0x0E7F,
	0x0E3F,0x065F,0x063F,0x1419,0x0005,0x0006,0x000D,0x000F,
	0x000F,0x000F,0x000E,0x000E,0x000E,0x000F,0x000F,0x0010,
	0x0030,0x0830,0x0830,0x0831,0x0853,0x0053,0x0854,0x0853,
	0x0052,0x0052,0x0052,0x0054,0x0854,0x0854,0x0854,0x0051,
	0x0032,0x0052,0x0853,0x0854,0x0854,0x0854,0x0032,0x0052,
	0x0032,0x0053,0x0854,0x0054,0x0854,0x0032,0x0052,0x0032,
	0x0053,0x0054,0x0854,0x0854,0x0032,0x0051,0x0031,0x0851,
	0x0852,0x0051,0x0031,0x000F,0x000D,0x000D,0x000E,0x000F,
	0x000F,0x000F,0x000E,0x000E,0x000B,0x0006,0x0005,0x0CB9,
	0x371F,0x66DF,0x4E7F,0x2E1F,0x2E1F,0xD7BF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x0BFD,0x061F,
	0x061F,0x065F,0x0E5F,0x1419,0x0005,0x0007,0x000C,0x000F,
	0x000F,0x0010,0x000F,0x000D,0x000D,0x000E,0x0871,0x1092,
	0x10B3,0x10B3,0x1092,0x1092,0x1093,0x10B5,0x10B6,0x1097,
	0x10B7,0x1095,0x1096,0x1096,0x10B8,0x10B9,0x10B9,0x10B9,
	0x1097,0x1097,0x10B6,0x1098,0x10B9,0x10B9,0x10B9,0x1096,
	0x1096,0x1096,0x10B8,0x10B9,0x10B9,0x10B9,0x1097,0x1096,
	0x1096,0x1098,0x10B8,0x10B8,0x10B7,0x1095,0x1094,0x1093,
	0x1094,0x10B5,0x10B4,0x10B4,0x10B1,0x1091,0x084F,0x000E,
	0x000F,0x000F,0x000F,0x000E,0x000D,0x000D,0x000D,0x0007,
	0x0005,0x1499,0x3F1F,0x56BF,0x365F,0x25DF,0x463F,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x6D7E,0x051F,
	0x0E1F,0x061F,0x065F,0x14DD,0x0005,0x0007,0x000D,0x000D,
	0x000F,0x000F,0x000F,0x000E,0x000D,0x086F,0x1090,0x10B3,
	0x10B3,0x10B4,0x1093,0x0873,0x1093,0x1094,0x10B6,0x10B7,
	0x1098,0x10B8,0x0896,0x1097,0x1097,0x109A,0x109C,0x109C,
	0x109C,0x089A,0x109A,0x109A,0x109D,0x10BE,0x109E,0x10BE,
	0x109B,0x109B,0x109B,0x109D,0x10BD,0x109D,0x10BC,0x1099,
	0x1099,0x1098,0x1099,0x109A,0x10B9,0x10B8,0x0896,0x1095,
	0x1094,0x1095,0x1096,0x10B5,0x1094,0x1091,0x1091,0x1090,
	0x1091,0x0871,0x000F,0x000F,0x000E,0x000E,0x000D,0x000E,
	0x000E,0x0007,0x0005,0x0D7D,0x4EBF,0x467F,0x35FF,0x15DF,
	0x8EDF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xEFBF,0x03FE,
	0x0DFF,0x05FF,0x061F,0x15DF,0x0007,0x0007,0x000C,0x000D,
	0x000E,0x0010,0x000F,0x000F,0x000E,0x1090,0x1090,0x1091,
	0x1093,0x1094,0x1095,0x1094,0x1093,0x1094,0x1094,0x1097,
	0x1098,0x1099,0x1098,0x1097,0x1098,0x1098,0x109B,0x109C,
	0x109D,0x109D,0x109B,0x109B,0x109C,0x109E,0x109F,0x109F,
	0x10BF,0x089C,0x109C,0x109C,0x109E,0x109F,0x109E,0x10BD,
	0x109A,0x1099,0x1098,0x1099,0x109A,0x109A,0x10B9,0x0896,
	0x0895,0x1095,0x1095,0x1096,0x10B5,0x1095,0x1092,0x1092,
	0x1091,0x1091,0x10B2,0x1092,0x0010,0x000E,0x000D,0x000E,
	0x000E,0x000F,0x000E,0x0007,0x0008,0x165F,0x4E9F,0x363F,
	0x25DF,0x1D9F,0xEFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0x5D3E,
	0x051F,0x05DF,0x0DFF,0x0E5F,0x09F1,0x0005,0x000B,0x000D,
	0x000E,0x000E,0x000F,0x000F,0x0010,0x0850,0x1090,0x1090,
	0x1091,0x1094,0x1094,0x10B5,0x1095,0x1093,0x1094,0x1095,
	0x1097,0x1099,0x1099,0x1099,0x1097,0x1098,0x0898,0x109B,
	0x10BD,0x109D,0x109D,0x109B,0x109C,0x109C,0x109F,0x10BF,
	0x109F,0x10BF,0x089C,0x109C,0x109C,0x109E,0x109F,0x109E,
	0x10BD,0x107A,0x1099,0x1099,0x109A,0x109B,0x109A,0x10BA,
	0x1097,0x1096,0x1095,0x1096,0x1097,0x10B6,0x10B6,0x1093,
	0x1092,0x1091,0x1092,0x10B3,0x10B2,0x0871,0x000E,0x000D,
	0x000E,0x000E,0x000F,0x000F,0x000C,0x0005,0x0211,0x2E7F,
	0x465F,0x2DFF,0x0D9F,0x769F,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0x03DF,0x057F,0x0D9F,0x0E1F,0x0CFD,0x0004,0x0009,0x000F,
	0x000D,0x000E,0x000E,0x000F,0x000F,0x0010,0x1091,0x1090,
	0x1091,0x1092,0x1094,0x10B5,0x10B5,0x1095,0x1094,0x1095,
	0x1095,0x10B8,0x1099,0x10B9,0x10B9,0x1097,0x1098,0x0898,
	0x109B,0x10BD,0x109D,0x109D,0x109B,0x109B,0x109B,0x109E,
	0x109F,0x109F,0x109F,0x089C,0x109C,0x109C,0x109E,0x109F,
	0x109F,0x10BD,0x109A,0x109A,0x1099,0x109A,0x10BB,0x109A,
	0x109A,0x1097,0x1096,0x1096,0x1096,0x1097,0x1096,0x10B6,
	0x1093,0x1093,0x1092,0x1092,0x10B3,0x1092,0x1092,0x000F,
	0x000E,0x000D,0x000E,0x0010,0x000F,0x000F,0x0009,0x0004,
	0x0C9D,0x469F,0x361F,0x1DDF,0x159F,0xF7FF,0xFFFF,0xFFFF,
	0x861E,0x045F,0x055F,0x059F,0x0E1F,0x096E,0x0006,0x000E,
	0x000E,0x000E,0x000E,0x000D,0x000F,0x0010,0x0010,0x0871,
	0x1091,0x1091,0x1092,0x10B5,0x1095,0x1096,0x1095,0x0894,
	0x1095,0x0895,0x10B8,0x1099,0x10BA,0x10B9,0x1098,0x1098,
	0x1099,0x109C,0x109D,0x10BD,0x10BD,0x109B,0x109B,0x109B,
	0x109E,0x10BF,0x109F,0x10BF,0x109C,0x109C,0x109B,0x109E,
	0x109F,0x109E,0x10BD,0x109A,0x109A,0x1099,0x109A,0x10BB,
	0x109B,0x10BA,0x1097,0x1096,0x1096,0x1096,0x1098,0x1097,
	0x1096,0x1094,0x1093,0x1092,0x1093,0x10B4,0x10B3,0x0872,
	0x000F,0x000E,0x000D,0x000E,0x0010,0x000F,0x000F,0x000D,
	0x0006,0x012E,0x25BF,0x3E5F,0x25DF,0x0D9F,0x9EFF,0xFFFF,
	0xFFFF,0x2C7E,0x049F,0x0D3F,0x059F,0x155E,0x0005,0x0009,
	0x0010,0x000E,0x000E,0x000D,0x000D,0x0010,0x0010,0x0011,
	0x0011,0x1091,0x1091,0x1092,0x1095,0x10B5,0x1096,0x10B6,
	0x1095,0x1095,0x1095,0x10B8,0x1099,0x109A,0x109A,0x1098,
	0x1099,0x1099,0x109C,0x109D,0x109D,0x109D,0x109A,0x109B,
	0x109B,0x109D,0x10BE,0x109F,0x109E,0x109B,0x109B,0x109B,
	0x109D,0x109E,0x109E,0x10BD,0x109A,0x109A,0x1099,0x109A,
	0x10BB,0x10BB,0x109A,0x1097,0x1096,0x1096,0x1097,0x1098,
	0x10B7,0x1097,0x1094,0x0893,0x1092,0x1093,0x10B4,0x10B3,
	0x0012,0x0010,0x000E,0x000E,0x000D,0x000F,0x000F,0x000F,
	0x000E,0x0009,0x0005,0x0BFE,0x469F,0x2DFF,0x159F,0x461F,
	0xFFFF,0xFFFF,0x039E,0x047F,0x0D1F,0x05BF,0x12D4,0x0005,
	0x000D,0x000F,0x000E,0x000E,0x000D,0x000D,0x0010,0x0010,
	0x0011,0x0011,0x0010,0x1092,0x1092,0x10B5,0x1096,0x1096,
	0x1096,0x1095,0x1095,0x1096,0x10B8,0x1099,0x109A,0x1099,
	0x1098,0x1098,0x1098,0x109B,0x109D,0x109D,0x109C,0x109A,
	0x109A,0x109A,0x109D,0x109E,0x109E,0x109D,0x109B,0x109B,
	0x109A,0x109D,0x109E,0x109D,0x10BD,0x109A,0x1099,0x1099,
	0x109A,0x10BB,0x10BB,0x10BA,0x1097,0x1096,0x1096,0x1097,
	0x1098,0x10B7,0x1097,0x1094,0x1093,0x1092,0x1093,0x1094,
	0x0012,0x0012,0x0010,0x000F,0x000E,0x000E,0x000F,0x0010,
	0x0010,0x000E,0x000B,0x0006,0x01F4,0x35FF,0x363F,0x1DBF,
	0x0D7F,0xFFFF,0xBEFF,0x039F,0x043F,0x0CDF,0x0DDF,0x00CB,
	0x0007,0x000F,0x000F,0x000F,0x000D,0x000D,0x000E,0x0010,
	0x0010,0x0012,0x0011,0x0010,0x0011,0x0032,0x1095,0x10B6,
	0x10B6,0x10B6,0x1095,0x1095,0x1096,0x10B8,0x1099,0x10BA,
	0x10B9,0x1098,0x1098,0x1098,0x10BB,0x10BC,0x10BD,0x10BC,
	0x109A,0x109A,0x109A,0x10BC,0x109D,0x109D,0x109D,0x109A,
	0x109A,0x109A,0x109C,0x10BD,0x109D,0x10BD,0x109A,0x1099,
	0x1099,0x109A,0x10BB,0x109A,0x10BA,0x1097,0x1096,0x1096,
	0x1097,0x1098,0x10B7,0x1097,0x1094,0x1094,0x0893,0x0033,
	0x0013,0x0012,0x0012,0x0010,0x000F,0x000E,0x000E,0x000F,
	0x000F,0x0010,0x000E,0x000D,0x0007,0x006B,0x255F,0x465F,
	0x25DF,0x0D9F,0xC77F,0x861F,0x03BF,0x041F,0x04BF,0x159F,
	0x0005,0x0009,0x000F,0x000F,0x000F,0x000D,0x000D,0x000E,
	0x0010,0x0011,0x0012,0x0011,0x0011,0x0011,0x0011,0x0014,
	0x0015,0x0036,0x0855,0x0874,0x0875,0x0875,0x1098,0x0879,
	0x089A,0x0879,0x0877,0x0878,0x0878,0x087B,0x087C,0x087C,
	0x087C,0x0879,0x087A,0x0879,0x087C,0x087D,0x087D,0x109C,
	0x0879,0x087A,0x0879,0x087B,0x087C,0x087D,0x089C,0x0879,
	0x0879,0x0878,0x087A,0x089B,0x087A,0x109A,0x0877,0x0876,
	0x0876,0x0876,0x0878,0x0877,0x0857,0x0033,0x0012,0x0012,
	0x0012,0x0013,0x0013,0x0012,0x0010,0x000F,0x000E,0x000E,
	0x000F,0x000F,0x0010,0x000E,0x000D,0x0009,0x0006,0x145F,
	0x4E9F,0x2E1F,0x0D9F,0x96FF,0x5D5E,0x03DF,0x03FF,0x04BF,
	0x149C,0x0004,0x000B,0x000F,0x000F,0x000F,0x000D,0x000E,
	0x000E,0x0010,0x0011,0x0012,0x0011,0x0011,0x0011,0x0012,
	0x0014,0x0015,0x0015,0x0015,0x0014,0x0015,0x0015,0x0017,
	0x0018,0x0019,0x0018,0x0017,0x0017,0x0018,0x001A,0x001B,
	0x001B,0x001B,0x0018,0x0019,0x0018,0x001B,0x001C,0x001C,
	0x001B,0x0019,0x0019,0x0018,0x001A,0x001C,0x001C,0x001B,
	0x0019,0x0018,0x0018,0x0019,0x001A,0x0019,0x0019,0x0016,
	0x0016,0x0015,0x0016,0x0017,0x0016,0x0016,0x0013,0x0013,
	0x0012,0x0012,0x0013,0x0012,0x0012,0x0010,0x000F,0x000F,
	0x000E,0x000F,0x000F,0x0010,0x000E,0x000D,0x000A,0x0005,
	0x0B3B,0x4E9F,0x363F,0x15BF,0x76BF,0x3CDE,0x03FF,0x03FF,
	0x049F,0x13B7,0x0005,0x000D,0x000F,0x0010,0x000F,0x000D,
	0x000E,0x000E,0x0011,0x0011,0x0012,0x0011,0x0011,0x0011,
	0x0012,0x0014,0x0015,0x0015,0x0015,0x0013,0x0014,0x0015,
	0x0017,0x0018,0x0019,0x0018,0x0016,0x0017,0x0017,0x001A,
	0x001B,0x001A,0x001A,0x0018,0x0018,0x0018,0x001A,0x001B,
	0x001B,0x001A,0x0018,0x0018,0x0018,0x001A,0x001B,0x001B,
	0x001A,0x0018,0x0018,0x0017,0x0019,0x001A,0x0019,0x0019,
	0x0016,0x0015,0x0015,0x0016,0x0017,0x0016,0x0016,0x0013,
	0x0013,0x0012,0x0012,0x0013,0x0012,0x0012,0x0010,0x000F,
	0x000F,0x000E,0x000F,0x000F,0x0010,0x000E,0x000E,0x000C,
	0x0005,0x0A77,0x467F,0x3E5F,0x25DF,0x5E5F,0x2C7F,0x03FF,
	0x03FF,0x049F,0x0B15,0x0005,0x000D,0x000F,0x0010,0x000F,
	0x000D,0x000E,0x000E,0x0011,0x0011,0x0012,0x0011,0x0011,
	0x0011,0x0012,0x0014,0x0015,0x0015,0x0015,0x0014,0x0014,
	0x0015,0x0017,0x0018,0x0018,0x0018,0x0016,0x0017,0x0017,
	0x0019,0x001A,0x001A,0x001A,0x0017,0x0018,0x0017,0x0019,
	0x001A,0x001A,0x001A,0x0017,0x0017,0x0017,0x0019,0x001A,
	0x001A,0x001A,0x0017,0x0017,0x0017,0x0018,0x001A,0x0019,
	0x0019,0x0016,0x0015,0x0015,0x0016,0x0017,0x0016,0x0016,
	0x0013,0x0013,0x0012,0x0012,0x0013,0x0012,0x0012,0x0010,
	0x000F,0x000F,0x000E,0x000F,0x000F,0x0010,0x000E,0x000D,
	0x000C,0x0005,0x01F5,0x465F,0x467F,0x2DFF,0x563F,0x247F,
	0x03DF,0x03FF,0x04BF,0x0AF4,0x0005,0x000E,0x000F,0x0010,
	0x000F,0x000D,0x000E,0x000E,0x0011,0x0011,0x0012,0x0011,
	0x0011,0x0011,0x0012,0x0014,0x0015,0x0015,0x0014,0x0013,
	0x0014,0x0014,0x0017,0x0018,0x0018,0x0017,0x0016,0x0016,
	0x0016,0x0019,0x0019,0x0019,0x0019,0x0017,0x0017,0x0017,
	0x0019,0x001A,0x001A,0x0019,0x0017,0x0017,0x0016,0x0018,
	0x0019,0x0019,0x0019,0x0017,0x0017,0x0016,0x0018,0x0019,
	0x0019,0x0018,0x0016,0x0015,0x0015,0x0015,0x0017,0x0016,
	0x0016,0x0013,0x0013,0x0012,0x0012,0x0013,0x0012,0x0012,
	0x0010,0x000F,0x000F,0x000F,0x000F,0x000F,0x0010,0x000F,
	0x000D,0x000C,0x0005,0x01D4,0x465F,0x4E9F,0x2E1F,0x565F,
	0x2C7F,0x03FF,0x03FF,0x049F,0x0AF4,0x0005,0x000E,0x000F,
	0x0010,0x000F,0x000D,0x000E,0x000E,0x0010,0x0011,0x0012,
	0x0011,0x0010,0x0011,0x0011,0x0014,0x0014,0x0015,0x0014,
	0x0013,0x0014,0x0014,0x0016,0x0017,0x0018,0x0017,0x0015,
	0x0016,0x0016,0x0018,0x0019,0x0019,0x0018,0x0016,0x0016,
	0x0016,0x0018,0x0019,0x0019,0x0018,0x0016,0x0016,0x0016,
	0x0018,0x0019,0x0019,0x0018,0x0016,0x0016,0x0016,0x0017,
	0x0018,0x0018,0x0018,0x0015,0x0015,0x0014,0x0015,0x0016,
	0x0015,0x0015,0x0013,0x0012,0x0012,0x0012,0x0013,0x0012,
	0x0012,0x0010,0x000F,0x000F,0x000E,0x000F,0x000F,0x0010,
	0x000F,0x000D,0x000C,0x0005,0x01D4,0x4E7F,0x56BF,0x363F,
	0x5E7F,0x34BE,0x03FF,0x03FF,0x049F,0x1356,0x0005,0x000D,
	0x000F,0x0010,0x000F,0x000D,0x000E,0x000E,0x0010,0x0010,
	0x0011,0x0011,0x0010,0x0011,0x0011,0x0014,0x0014,0x0015,
	0x0014,0x0013,0x0014,0x0014,0x0016,0x0017,0x0017,0x0017,
	0x0015,0x0015,0x0015,0x0018,0x0018,0x0018,0x0018,0x0015,
	0x0016,0x0015,0x0017,0x0018,0x0018,0x0018,0x0015,0x0015,
	0x0015,0x0017,0x0018,0x0018,0x0018,0x0016,0x0015,0x0015,
	0x0017,0x0018,0x0017,0x0017,0x0015,0x0014,0x0014,0x0015,
	0x0016,0x0015,0x0015,0x0012,0x0012,0x0011,0x0012,0x0013,
	0x0012,0x0012,0x0010,0x000F,0x000F,0x000E,0x000F,0x000F,
	0x0010,0x000F,0x000D,0x000C,0x0005,0x0235,0x56BF,0x56BF,
	0x3E5F,0x669F,0x4D1E,0x03DF,0x0BFF,0x045F,0x1C59,0x0005,
	0x000C,0x000F,0x0010,0x000F,0x000D,0x000E,0x000E,0x0010,
	0x0010,0x0011,0x0011,0x0010,0x0011,0x0011,0x0013,0x0014,
	0x0014,0x0014,0x0013,0x0013,0x0013,0x0016,0x0017,0x0017,
	0x0016,0x0014,0x0015,0x0015,0x0017,0x0017,0x0017,0x0017,
	0x0015,0x0015,0x0015,0x0017,0x0017,0x0017,0x0017,0x0015,
	0x0015,0x0015,0x0016,0x0017,0x0017,0x0017,0x0015,0x0015,
	0x0015,0x0016,0x0017,0x0017,0x0017,0x0014,0x0014,0x0014,
	0x0014,0x0016,0x0015,0x0015,0x0012,0x0012,0x0011,0x0011,
	0x0013,0x0012,0x0012,0x0010,0x000F,0x000E,0x000E,0x000F,
	0x000F,0x0010,0x000F,0x000E,0x000B,0x0005,0x02B9,0x6F1F,
	0x5EDF,0x3E7F,0x7EDF,0x6D9F,0x03DF,0x03FF,0x03FF,0x2DBE,
	0x0005,0x000A,0x000F,0x0010,0x000F,0x000D,0x000D,0x000E,
	0x0010,0x0010,0x0011,0x0010,0x0010,0x0010,0x0011,0x0013,
	0x0014,0x0014,0x0014,0x0012,0x0013,0x0013,0x0015,0x0016,
	0x0016,0x0016,0x0014,0x0014,0x0014,0x0016,0x0017,0x0017,
	0x0016,0x0014,0x0014,0x0014,0x0016,0x0016,0x0017,0x0016,
	0x0014,0x0014,0x0014,0x0016,0x0017,0x0017,0x0016,0x0014,
	0x0014,0x0014,0x0015,0x0017,0x0016,0x0016,0x0014,0x0013,
	0x0013,0x0014,0x0015,0x0015,0x0014,0x0012,0x0012,0x0011,
	0x0011,0x0013,0x0012,0x0012,0x000F,0x000F,0x000E,0x000E,
	0x000F,0x000F,0x0010,0x000E,0x000E,0x0009,0x0005,0x0B9D,
	0x7F9F,0x5EFF,0x467F,0x9F3F,0x9E5E,0x03BF,0x03FF,0x03DF,
	0x3E5F,0x0007,0x0008,0x000F,0x000F,0x000F,0x000D,0x000D,
	0x000E,0x0010,0x0010,0x0011,0x0010,0x000F,0x0010,0x0011,
	0x0013,0x0013,0x0014,0x0013,0x0012,0x0013,0x0012,0x0015,
	0x0015,0x0016,0x0015,0x0013,0x0014,0x0013,0x0015,0x0016,
	0x0016,0x0015,0x0013,0x0014,0x0013,0x0015,0x0016,0x0016,
	0x0016,0x0013,0x0013,0x0013,0x0015,0x0016,0x0016,0x0016,
	0x0014,0x0013,0x0013,0x0015,0x0016,0x0016,0x0016,0x0014,
	0x0013,0x0013,0x0013,0x0015,0x0014,0x0014,0x0012,0x0011,
	0x0011,0x0011,0x0012,0x0011,0x0011,0x000F,0x000E,0x000E,
	0x000E,0x000F,0x000F,0x0010,0x000E,0x000E,0x0008,0x0008,
	0x251F,0x8F9F,0x66FF,0x4EBF,0xBF7E,0xD71F,0x039F,0x03FF,
	0x03DF,0x25BF,0x222F,0x0006,0x000E,0x000F,0x000F,0x000E,
	0x000D,0x000D,0x0010,0x0010,0x0011,0x0010,0x000F,0x0010,
	0x0010,0x0013,0x0013,0x0013,0x0013,0x0011,0x0012,0x0012,
	0x0014,0x0015,0x0015,0x0015,0x0013,0x0013,0x0013,0x0015,
	0x0015,0x0015,0x0015,0x0013,0x0013,0x0013,0x0014,0x0015,
	0x0015,0x0015,0x0013,0x0013,0x0013,0x0014,0x0015,0x0015,
	0x0015,0x0013,0x0013,0x0013,0x0014,0x0015,0x0015,0x0015,
	0x0013,0x0013,0x0012,0x0013,0x0014,0x0014,0x0014,0x0011,
	0x0011,0x0011,0x0011,0x0012,0x0011,0x0011,0x000F,0x000E,
	0x000E,0x000E,0x000E,0x0010,0x0010,0x000E,0x000D,0x0006,
	0x010F,0x563F,0x87BF,0x6F1F,0x5EDF,0xDF7F,0xFFFF,0x03DE,
	0x03FF,0x03FF,0x0C7F,0x5559,0x0004,0x000B,0x0010,0x000E,
	0x000E,0x000D,0x000D,0x000F,0x0010,0x0010,0x0010,0x000F,
	0x0010,0x0010,0x0012,0x0013,0x0013,0x0012,0x0011,0x0012,
	0x0012,0x0014,0x0014,0x0014,0x0014,0x0012,0x0012,0x0012,
	0x0014,0x0015,0x0014,0x0014,0x0012,0x0012,0x0012,0x0014,
	0x0014,0x0015,0x0014,0x0012,0x0012,0x0012,0x0014,0x0015,
	0x0015,0x0014,0x0012,0x0012,0x0012,0x0013,0x0015,0x0014,
	0x0014,0x0012,0x0012,0x0012,0x0013,0x0014,0x0013,0x0013,
	0x0011,0x0011,0x0010,0x0010,0x0012,0x0011,0x0011,0x000F,
	0x000E,0x000E,0x000E,0x000F,0x0010,0x000F,0x000E,0x000A,
	0x0005,0x0299,0x977F,0x8F9F,0x771F,0x771F,0xFF9E,0xFFDE,
	0x4CFE,0x03DF,0x03FF,0x03BF,0x6F3F,0x0027,0x0007,0x000F,
	0x000E,0x000E,0x000E,0x000D,0x000F,0x000F,0x0010,0x000F,
	0x000F,0x000F,0x0010,0x0012,0x0012,0x0012,0x0012,0x0011,
	0x0011,0x0011,0x0013,0x0014,0x0014,0x0013,0x0011,0x0011,
	0x0011,0x0013,0x0014,0x0014,0x0013,0x0011,0x0012,0x0011,
	0x0013,0x0014,0x0014,0x0013,0x0011,0x0012,0x0011,0x0013,
	0x0014,0x0014,0x0013,0x0012,0x0011,0x0012,0x0013,0x0014,
	0x0014,0x0014,0x0012,0x0011,0x0011,0x0012,0x0013,0x0013,
	0x0013,0x0010,0x0010,0x0010,0x0010,0x0011,0x0010,0x0010,
	0x000F,0x000E,0x000D,0x000D,0x0010,0x0010,0x000F,0x000E,
	0x0007,0x0008,0x2CFF,0xC7FF,0x979F,0x775F,0xA75E,0xFF9E,
	0xFFDE,0xAE5D,0x039F,0x03FF,0x03DF,0x357F,0x5C96,0x0004,
	0x000C,0x000E,0x000E,0x000F,0x000D,0x000F,0x000F,0x0010,
	0x000F,0x000F,0x000F,0x000F,0x0011,0x0012,0x0012,0x0011,
	0x0010,0x0011,0x0011,0x0013,0x0013,0x0013,0x0012,0x0011,
	0x0011,0x0011,0x0013,0x0013,0x0013,0x0013,0x0011,0x0011,
	0x0011,0x0012,0x0013,0x0013,0x0013,0x0011,0x0011,0x0011,
	0x0012,0x0013,0x0013,0x0013,0x0011,0x0011,0x0011,0x0012,
	0x0013,0x0013,0x0013,0x0011,0x0011,0x0011,0x0011,0x0013,
	0x0012,0x0012,0x0010,0x0010,0x000F,0x0010,0x0011,0x0010,
	0x0010,0x000E,0x000E,0x000D,0x000E,0x0010,0x0010,0x000F,
	0x000B,0x0005,0x01F6,0x96FF,0xBFDF,0xA77F,0x879F,0xCF3D,
	0xFFBE,0xFFDE,0xEF7C,0x141E,0x03FF,0x03FF,0x039F,0xA7FF,
	0x0047,0x0007,0x000E,0x000D,0x000E,0x000E,0x000F,0x000E,
	0x000F,0x000F,0x000E,0x000F,0x000F,0x0011,0x0011,0x0012,
	0x0011,0x0010,0x0010,0x0010,0x0012,0x0012,0x0013,0x0012,
	0x0010,0x0010,0x0010,0x0012,0x0012,0x0012,0x0012,0x0010,
	0x0011,0x0010,0x0012,0x0012,0x0012,0x0012,0x0010,0x0011,
	0x0010,0x0012,0x0012,0x0012,0x0012,0x0010,0x0010,0x0010,
	0x0011,0x0013,0x0012,0x0012,0x0010,0x0010,0x0010,0x0011,
	0x0012,0x0012,0x0012,0x0010,0x000F,0x000F,0x0010,0x0010,
	0x0010,0x0010,0x000E,0x000D,0x000D,0x000F,0x0010,0x000F,
	0x000F,0x0006,0x0008,0x2CBF,0xF7FF,0xC7BF,0xA79F,0xAF9F,
	0xE71C,0xFFDF,0xFFDF,0xEF7D,0x8DBC,0x03BF,0x03FF,0x03DF,
	0x34FF,0xBE9B,0x0004,0x0009,0x000D,0x000E,0x000E,0x0010,
	0x000F,0x000F,0x000E,0x000E,0x000E,0x000F,0x0010,0x0011,
	0x0011,0x0011,0x000F,0x0010,0x000F,0x0011,0x0012,0x0012,
	0x0011,0x000F,0x0010,0x0010,0x0011,0x0012,0x0011,0x0011,
	0x000F,0x0010,0x000F,0x0011,0x0012,0x0012,0x0011,0x0010,
	0x0010,0x000F,0x0011,0x0012,0x0012,0x0011,0x0010,0x0010,
	0x0010,0x0010,0x0012,0x0012,0x0012,0x0010,0x0010,0x000F,
	0x0010,0x0011,0x0011,0x0011,0x000F,0x000F,0x000E,0x000F,
	0x0010,0x0010,0x000F,0x000D,0x000D,0x000E,0x000F,0x000F,
	0x000F,0x0009,0x0004,0x02BA,0xC77F,0xF7FF,0xD7BF,0xBFDF,
	0xCF1C,0xEF3C,0xFFDF,0xFFFF,0xF77D,0xE71B,0x141E,0x03FF,
	0x03FF,0x039F,0xA73F,0x7391,0x0005,0x000A,0x000E,0x000E,
	0x0010,0x0010,0x000F,0x000D,0x000D,0x000E,0x000E,0x0010,
	0x0010,0x0010,0x0010,0x000F,0x000F,0x000F,0x0011,0x0011,
	0x0011,0x0010,0x000F,0x000F,0x000F,0x0010,0x0011,0x0011,
	0x0011,0x000F,0x000F,0x000F,0x0010,0x0011,0x0011,0x0011,
	0x000F,0x000F,0x000F,0x0010,0x0011,0x0011,0x0011,0x000F,
	0x000F,0x000F,0x0010,0x0011,0x0011,0x0011,0x000F,0x000F,
	0x000F,0x0010,0x0011,0x0011,0x0011,0x000F,0x000E,0x000E,
	0x000F,0x0010,0x000F,0x000F,0x000E,0x000F,0x000E,0x000E,
	0x0010,0x000B,0x0004,0x0151,0x55DF,0xFFFF,0xF7FF,0xD7DF,
	0xE7BF,0xDE9A,0xEF7D,0xFFFF,0xFFFF,0xF79E,0xEF1B,0xA5FB,
	0x03BF,0x03FF,0x03FF,0x039F,0xDFFF,0x298B,0x0005,0x000A,
	0x000E,0x000F,0x0010,0x0011,0x000F,0x000D,0x000D,0x000E,
	0x000F,0x0010,0x0010,0x0010,0x000E,0x000F,0x000E,0x0010,
	0x0010,0x0010,0x0010,0x000E,0x000E,0x000E,0x0010,0x0010,
	0x0010,0x0010,0x000E,0x000F,0x000E,0x0010,0x0010,0x0010,
	0x0010,0x000E,0x000E,0x000E,0x0010,0x0010,0x0010,0x0010,
	0x000E,0x000F,0x000F,0x000F,0x0010,0x0010,0x0010,0x000F,
	0x000E,0x000F,0x000F,0x0010,0x0010,0x0010,0x000E,0x000E,
	0x000E,0x000E,0x000F,0x000E,0x0010,0x000F,0x000F,0x000E,
	0x000E,0x000B,0x0005,0x008C,0x1CDF,0xB7DF,0xD7DF,0xFFFF,
	0xF7FF,0xD6BB,0xDEFB,0xF79E,0xFFFF,0xFFFF,0xFFDE,0xE73D,
	0xE6FA,0x4CBC,0x03BF,0x03FF,0x03DF,0x03FF,0x9FFF,0x112B,
	0x0005,0x000A,0x000F,0x0010,0x0010,0x0010,0x000F,0x000E,
	0x000D,0x000F,0x000F,0x000F,0x000F,0x000D,0x000E,0x000E,
	0x000F,0x000F,0x000F,0x000F,0x000E,0x000E,0x000E,0x000F,
	0x000F,0x000F,0x000F,0x000E,0x000E,0x000D,0x000F,0x000F,
	0x000F,0x000F,0x000E,0x000E,0x000E,0x000F,0x000F,0x000F,
	0x000F,0x000E,0x000E,0x000E,0x000F,0x000F,0x000F,0x000F,
	0x000E,0x000E,0x000E,0x000E,0x0010,0x000F,0x000F,0x000E,
	0x000D,0x000D,0x000E,0x0010,0x0011,0x0011,0x000F,0x000E,
	0x000D,0x000A,0x0005,0x006C,0x0C5F,0x6F7F,0x9F9F,0xCFBF,
	0xE7FF,0xD73C,0xD679,0xE73C,0xF7DF,0xFFFF,0xFFFF,0xFFFF,
	0xEF7D,0xDEFB,0xD679,0x1C1E,0x03DF,0x03FF,0x03DF,0x047F,
	0x4FBF,0x09ED,0x0004,0x0008,0x000E,0x0010,0x000F,0x000F,
	0x000F,0x000F,0x0010,0x0010,0x000F,0x000E,0x000D,0x000D,
	0x000D,0x000E,0x000F,0x000F,0x000E,0x000D,0x000D,0x000D,
	0x000E,0x000F,0x000F,0x000E,0x000D,0x000D,0x000D,0x000E,
	0x000F,0x000F,0x000E,0x000D,0x000D,0x000D,0x000E,0x000F,
	0x000F,0x000F,0x000D,0x000D,0x000D,0x000E,0x000F,0x000F,
	0x000F,0x000D,0x000D,0x000D,0x000D,0x000F,0x000E,0x000F,
	0x000E,0x000E,0x000F,0x0010,0x0011,0x0011,0x0010,0x000E,
	0x000C,0x0008,0x0004,0x08CE,0x0C7F,0x2F1F,0x5F3F,0x8F7F,
	0xB7BF,0xA77E,0xC638,0xDEFB,0xEF7D,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xF7BE,0xE73C,0xDEBA,0xB619,0x03DF,0x03FF,0x0BFF,
	0x03FF,0x04BF,0x175F,0x13D5,0x0004,0x0006,0x000B,0x000E,
	0x000E,0x000F,0x000F,0x0011,0x0012,0x0012,0x0012,0x0011,
	0x0012,0x0012,0x0015,0x0015,0x0015,0x0015,0x0014,0x0014,
	0x0014,0x0016,0x0016,0x0036,0x0036,0x0014,0x0014,0x0014,
	0x0015,0x0017,0x0016,0x0036,0x0014,0x0014,0x0014,0x0016,
	0x0017,0x0016,0x0036,0x0014,0x0014,0x0014,0x0015,0x0016,
	0x0016,0x0036,0x0013,0x0013,0x0012,0x0013,0x0014,0x0013,
	0x0013,0x0011,0x0010,0x000F,0x000F,0x0010,0x0010,0x000E,
	0x000A,0x0006,0x0005,0x09F4,0x0CDF,0x0EFF,0x1F1F,0x4F1F,
	0x777F,0x675F,0xBE18,0xD69A,0xE73C,0xF7BE,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xEF7D,0xDEFB,0xD699,0xA5B9,0x03DF,
	0x03FF,0x03FF,0x0C5F,0x0CDF,0x069F,0x1E1E,0x094C,0x0004,
	0x0006,0x0009,0x000B,0x000D,0x0010,0x0011,0x0011,0x0011,
	0x0010,0x0010,0x0010,0x0012,0x0013,0x0013,0x0012,0x0011,
	0x0011,0x0011,0x0012,0x0013,0x0013,0x0013,0x0011,0x0011,
	0x0011,0x0012,0x0013,0x0013,0x0013,0x0011,0x0011,0x0011,
	0x0012,0x0013,0x0013,0x0013,0x0011,0x0011,0x0011,0x0012,
	0x0013,0x0013,0x0013,0x0011,0x0011,0x0011,0x0011,0x0013,
	0x0012,0x0012,0x0010,0x000F,0x000E,0x000E,0x000D,0x0009,
	0x0006,0x0005,0x00AC,0x0B9D,0x0D7F,0x0EFF,0x0EDF,0x0EFF,
	0x373F,0x2F1F,0xA5F8,0xD658,0xDEFB,0xEF7D,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFDF,0xEF5D,0xDEDB,0xCE58,
	0xA598,0x0BFE,0x03DF,0x043F,0x04DF,0x0D1F,0x05DF,0x0EDF,
	0x153C,0x09AE,0x0005,0x0005,0x0006,0x0007,0x0009,0x0009,
	0x000A,0x000A,0x000A,0x0009,0x000A,0x000A,0x000A,0x000A,
	0x0009,0x0009,0x0009,0x000A,0x000A,0x000A,0x000A,0x0009,
	0x0009,0x0009,0x000A,0x000A,0x000A,0x000A,0x0009,0x0009,
	0x0009,0x000A,0x000A,0x000A,0x000A,0x0009,0x0009,0x0009,
	0x0009,0x000A,0x000A,0x000A,0x0009,0x0009,0x000A,0x000A,
	0x000A,0x000A,0x000A,0x0009,0x0008,0x0007,0x0006,0x0005,
	0x0005,0x010E,0x0B3B,0x04FF,0x0E3F,0x06BF,0x0E9F,0x06DF,
	0x06FF,0x16DF,0x95D8,0xCE58,0xDEBB,0xEF5D,0xFFDF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xF7BE,0xE73C,
	0xD6BA,0xCE38,0xADB7,0x1C1D,0x03FF,0x04BF,0x0D3F,0x057F,
	0x0DBF,0x061F,0x0E7F,0x15FF,0x0BF8,0x0A31,0x08EB,0x0028,
	0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,
	0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,
	0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,
	0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,
	0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0007,0x0006,
	0x0007,0x0007,0x0007,0x0007,0x0007,0x0028,0x08CC,0x09B1,
	0x0A98,0x0C3F,0x0D1F,0x0DFF,0x0E7F,0x065F,0x067F,0x0E9F,
	0x06BF,0x267E,0xA5B6,0xD618,0xD6BA,0xE73C,0xF7BE,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xF79E,0xE71C,0xCE9A,0xCE38,0xBDB6,0x4C9A,0x03FF,0x04DF,
	0x057F,0x0D9F,0x05DF,0x0E1F,0x063F,0x063F,0x063F,0x0E1F,
	0x0DBF,0x0D7F,0x0D5F,0x0D3F,0x0D3F,0x0D1F,0x0CFF,0x0CBF,
	0x049F,0x0C5F,0x0C1F,0x0BDF,0x03DF,0x0C3F,0x0C7F,0x04BF,
	0x0CFF,0x0D1F,0x0D5F,0x0D7F,0x0D9F,0x0DBF,0x0DDF,0x0DDF,
	0x15FF,0x0E1F,0x163F,0x163F,0x165F,0x165F,0x165F,0x167F,
	0x167F,0x167F,0x167F,0x167F,0x169F,0x169F,0x167F,0x167F,
	0x167F,0x167F,0x167F,0x165F,0x0E1F,0x15FF,0x0DBF,0x0D5F,
	0x053F,0x057F,0x05FF,0x061F,0x063F,0x063F,0x0E5F,0x067F,
	0x067F,0x4DDB,0xBD95,0xCE18,0xD6BA,0xE71C,0xF79E,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xF79E,0xEF1C,0xCEBA,0xCE38,0xC5D6,0x8D16,
	0x241C,0x047F,0x057F,0x05FF,0x0DFF,0x061F,0x063F,0x0E3F,
	0x065F,0x0E5F,0x0E5F,0x0E7F,0x067F,0x0E5F,0x067F,0x067F,
	0x067F,0x0E7F,0x0E7F,0x0E5F,0x0E5F,0x0E5F,0x0E3F,0x061F,
	0x0DFF,0x0DFF,0x0DDF,0x05BF,0x059F,0x057F,0x055F,0x053F,
	0x051F,0x04FF,0x04BF,0x049F,0x047F,0x041F,0x03DF,0x039F,
	0x035F,0x033F,0x035F,0x03BF,0x03DF,0x041F,0x043F,0x047F,
	0x049F,0x04BF,0x04DF,0x04DF,0x04FF,0x04FF,0x051F,0x055F,
	0x057F,0x059F,0x0DBF,0x0DBF,0x05FF,0x061F,0x063F,0x061F,
	0x25FD,0x9556,0xC5B5,0xC638,0xD69A,0xE71C,0xF79E,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xF7BE,0xE73C,0xDEBB,0xC658,
	0xC5F7,0xB595,0x7CB7,0x241B,0x045F,0x057F,0x061F,0x0E5F,
	0x0E5F,0x0E5F,0x065F,0x065F,0x0E5F,0x065F,0x0E5F,0x0E5F,
	0x0E3F,0x0E5F,0x0E5F,0x0E3F,0x0E3F,0x0E3F,0x063F,0x061F,
	0x0DFF,0x05FF,0x05DF,0x05BF,0x059F,0x059F,0x057F,0x055F,
	0x053F,0x051F,0x04FF,0x04DF,0x04BF,0x047F,0x045F,0x041F,
	0x03DF,0x039F,0x033F,0x035F,0x03BF,0x03DF,0x041F,0x045F,
	0x047F,0x049F,0x04BF,0x04DF,0x04DF,0x04FF,0x04FF,0x051F,
	0x053F,0x055F,0x057F,0x059F,0x05DF,0x05DF,0x05DF,0x2D9C,
	0x7537,0xBD54,0xBDD7,0xCE59,0xDEBB,0xE73C,0xF7BE,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFDF,0xEF5D,
	0xDEFB,0xD69A,0xC618,0xC5D6,0xB574,0x94D5,0x5439,0x23FC,
	0x043F,0x04DF,0x055F,0x05DF,0x061F,0x063F,0x063F,0x063F,
	0x065F,0x065F,0x065F,0x065F,0x065F,0x065F,0x063F,0x063F,
	0x063F,0x061F,0x061F,0x05FF,0x05DF,0x05DF,0x05BF,0x059F,
	0x057F,0x055F,0x055F,0x053F,0x051F,0x04DF,0x04BF,0x047F,
	0x045F,0x041F,0x03DF,0x039F,0x037F,0x039F,0x03DF,0x03DF,
	0x03FF,0x043F,0x045F,0x049F,0x04BF,0x04DF,0x04FF,0x04FF,
	0x04FF,0x051F,0x051F,0x053F,0x055F,0x251D,0x5519,0x9514,
	0xBD54,0xC5B6,0xC638,0xCE9A,0xDEFB,0xEF5D,0xFFDF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xF7BE,0xE73C,0xDEDB,0xCE9A,0xC638,0xC5F6,0xBD95,
	0xB554,0xA4F3,0x84B5,0x6457,0x5478,0x4CB9,0x4D19,0x4D39,
	0x4D59,0x4D79,0x4D79,0x4D7A,0x4D7A,0x4D9A,0x4D9A,0x4D9A,
	0x4599,0x4D99,0x4D99,0x4D99,0x4D9A,0x4D99,0x4D99,0x4D99,
	0x4D7A,0x4D7A,0x4D5A,0x4D5A,0x4D59,0x4D39,0x4D3A,0x4D19,
	0x4D19,0x4D19,0x44F9,0x4CD9,0x4CD9,0x4CB9,0x4479,0x4439,
	0x4419,0x43F9,0x43F9,0x4419,0x4459,0x4479,0x4479,0x4499,
	0x44B9,0x4CD9,0x54D8,0x64D7,0x84D5,0xA4F3,0xB553,0xBD95,
	0xBDD7,0xC638,0xCE99,0xD6FB,0xEF3D,0xF7BE,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xF7BE,0xEF5D,0xE71C,0xD6BB,
	0xD67A,0xCE59,0xC618,0xC5F7,0xC5F6,0xC5D6,0xC5F6,0xC5D6,
	0xC5D6,0xC5D6,0xC5D6,0xC5D6,0xC5D6,0xC5D6,0xC5D6,0xC5D6,
	0xC5D6,0xC5D6,0xC5D6,0xC5D6,0xC5D6,0xC5D6,0xC5D6,0xC5D6,
	0xC5D6,0xC5D6,0xC5D6,0xC5D6,0xC5D6,0xC5D6,0xC5D6,0xC5D6,
	0xC5D6,0xC5D6,0xC5D6,0xC5D6,0xC5D6,0xC5D6,0xC5F6,0xC5F6,
	0xC5F6,0xC5F6,0xC5F6,0xC5F6,0xC5F6,0xC5F6,0xC5F6,0xC5F6,
	0xC5F6,0xC5D6,0xC5F6,0xC5D6,0xC5F7,0xC5F7,0xC618,0xCE59,
	0xCE7A,0xDEDA,0xE71C,0xEF5D,0xFFBE,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFDF,0xF7BE,0xEF7D,0xEF7D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,
	0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,
	0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,
	0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,
	0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,
	0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,
	0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF7D,
	0xEF7D,0xF7BE,0xFFDF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,
	0xFFFF,0xFFFF};
*/
