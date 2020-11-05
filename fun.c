#include <stdio.h>
#include <string.h> //提供strerror()函数
#include <errno.h>  //提供errno变量

int main(int argc, char *argv[])
{
        FILE *fps, *fpd; //定义两个流指针分别指向源文件和目标文件
            int ch; //保存读出的字符

                if (argc < 3) //检验命令行参数
                {
                            printf("Usage : %s <src_file> <dst_file>\n", argv[0]);
                                    return -1;
                                        
                }

                    /*
                     *         打开源文件
                     *             */
                    if ((fps = fopen(argv[1], "r")) == NULL)
                    {
                                //perror("fopen src file");
                                //        printf("fopen src file: %s\n",strerror(errno));
                                //                return -1;
                                //                    
                    }

                        /*
                         *         打开目标文件
                         *             */
                        if ((fpd = fopen(argv[2], "w")) == NULL)
                        {
                                    perror("fopen dst file");
                                            return -1;
                                                
                        }

                            while ((ch = fgetc(fps)) != EOF)
                            {
                                        fputc(ch, fpd);
                                            
                            }
                                fclose(fps);
                                    fclose(fpd);
                                        return 0;
                                        
}

