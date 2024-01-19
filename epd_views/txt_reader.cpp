#include "views.h"
// 引入File
using namespace std;

UWORD display_width = 296;  // 显示开始宽度
UWORD display_height = 128; // 显示开始高度
/*  生成txt文件索引,计算内容满一个可视区域的字节数
 *   @param path 文件路径
 *   文件索引格式：
 *   页码|偏移量|本页字节数
 */
void txt_index(const char *path, cFONT *font) {
  // 读取给定文件
  FILE *fp = fopen(path, "r");
  if (fp == NULL) {
    printf("open file failed\n");
    return;
  }
  // 构造索引文件名 文件名.i
  char index_path[strlen(path) + 3];
  strcpy(index_path, path);
  strcat(index_path, ".i");
  // 打开索引文件
  FILE *index = fopen(index_path, "w");
  if (index == NULL) {
    printf("open index file failed\n");
    return;
  }
  printf("index_path:%s\n", index_path);
  // 读取文件内容
  int ch;
  uint32_t page = 1;
  uint64_t offset = 0;
  uint32_t sum = 0;
  int width = 0;  // 当前行字符串宽度
  int height = 0; // 字符串高度
  // 读取下一个字符
  while ((ch = fgetc(fp)) != EOF) {
    // 判断当前字符属于ASCII还是汉字
    int chlen = getUTF8CharLength(ch);
    if (chlen == -1) {
      printf("invalid utf-8 sequence\n");
      return;
    }
    // 读取相应数量的字节
    char buffer[chlen + 1];
    buffer[0] = ch;
    for (int i = 1; i < chlen; i++) {
      ch = fgetc(fp);
      buffer[i] = ch;
    }
    buffer[chlen] = '\0';
    // 当前字符宽度
    int current_width = chlen == 1 ? font->ASCII_Width : font->Width;
    // 当遇到换行符或者宽度大于显示宽度时
    if (ch == '\n' || width + current_width > display_width) {
      // printf("width:%d height:%d sum:%d len:%d\n", width, height, sum,
      // chlen);
      printf("\n");
      width = 0;              // 重置宽度
      height += font->Height; // 换行(顶部高度)
      // 字符回退
      if (ch != '\n') {
        fseek(fp, -chlen, SEEK_CUR);
      } else {
        // 当前页字节数
        sum += chlen;
      }
      // 当前高度大于显示高度
      if (height > display_height - font->Height) {
        printf("\npage:%d offset:%ld sum:%d\n", page, offset, sum);
        printf("#######################\n");
        // 本页索引写入文件
        fprintf(index, "%d|%ld|%d\n", page, offset, sum);
        offset = ftell(fp); // 记录下一页起始偏移量
        sum = 0;            // 重置当前页字节数
        height = 0;         // 重置高度
        page++;             // 页码+1
      }
    } else {
      // 当前行宽度
      width += current_width;
      // 当前页字节数
      sum += chlen;
      printf("%s", buffer);
    }
    // printf("width:%d height:%d sum:%d len:%d\n", width, height, sum, chlen);
  }
  // 当最后一页字节数不为0时
  if (sum != 0) {
    // 本页字节数
    fprintf(index, "%d|%ld|%d\n", page, offset, sum);
    printf("\npage:%d offset:%ld sum:%d\n", page, offset, sum);
    printf("#######################\n");
  }
  // 关闭文件
  fclose(fp);
  fflush(index);
  fclose(index);
}

/**
 * 展示txt文件内容
 * @param filename 文件名
 * @param curr_page 页码
 */
void txt_view(const char *filename, uint32_t curr_page) {
  // 读取索引文件
  char index_path[strlen(filename) + 3];
  strcpy(index_path, filename);
  strcat(index_path, ".i");
  FILE *index = fopen(index_path, "r"); // 打开索引文件
  if (index == NULL) {
    printf("open index file failed\n");
    return;
  }
  // 根据索引文件读取内容
  uint32_t page = 1;               // 页码
  uint64_t offset = 0;             // 偏移量
  uint32_t sum = 0;                // 本页字节数
  FILE *fp = fopen(filename, "r"); // 打开txt文件
  if (fp == NULL) {
    printf("open file failed\n");
    return;
  }
  // 读取一页
  while (fscanf(index, "%d|%ld|%d\n", &page, &offset, &sum) != EOF) {
    if (page != curr_page) {
      continue;
    }
    printf("\npage:%d offset:%ld sum:%d\n", page, offset, sum); // 索引信息
    char *buffer = (char *)malloc((sum + 1) * sizeof(char)); // 单页面缓冲
    // 读取给定文件
    // 设置偏移量
    fseek(fp, offset, SEEK_SET);
    // 读取内容
    fread(buffer, sizeof(char), sum, fp);
    buffer[sum] = '\0';
    // 读取内容
    // Serial.println(buffer);
    printf("%s\n", buffer);
    Paint_Clear(WHITE);
    Paint_Draw_String(0, 0, buffer, &Font16CN, BLACK, WHITE, 0);
    free(buffer);
  }
  // 关闭文件
  fclose(fp);
}

void txt_reader(UBYTE *image) {
  display_width = Paint.Width;
  display_height = Paint.Height;
  printf("display_width:%d display_height:%d\n", display_width, display_height);
  Serial.println("txt_reader");
  // 测试索引
  const char path[] = "/home/silent/epaper/test.txt";
  printf("Create index file\n");
  txt_index(path, &Font16CN);
  // 展示内容
  printf("Show txt content\n");
  txt_view(path, 2);
}
