/**
 * Two-Phaseアルゴリズムのドライバー (C版)
 * http://kociemba.org/cube.htm
 * Version: 0.0.1
 *
 * 変更点(0.0.1):
 *     java1.4.4を移植しただけ
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "kttools.h"
#include "search.h"
#include "coordcube.h"
#include "cubiecube.h"

/**
 * メイン関数
 * コマンドライン引数(options):
 *   -n  スクランブルの数(1-10,000)
 *   -t  スクランブルの種類(0:通常 1:コーナーのみ 2:エッジのみ 3:パリティ有り 4:パリティ無し)
 *   -d  スクランブルの長さの最大値
 *   -l  タイムアウト値(秒)
 *   (-s  オプション有りで phase1/2 のセパレータ有り) 今のところ無し
 *   -v  オプション有りで 冗長出力
 *   -f  facelets文字列指定
 *   (-th スレッド数(1-4)) C版では無し
 *   -sd シード値(省略可)
 */
int main(int argc, char *argv[])
{
	// デフォルト値
	// スクランブルの数
	int num_scrambles = 5;
	// スクランブルの種類
	int type = 6;
	// スクランブルの長さの最大値
	int depth = 30;
	// タイムアウト値(ミリ秒)
	time_t limit = 5000;
	// 冗長出力
	int is_verbose = 0;
	// ステータス表示
	int show_status = 1;
	// Facelet文字列
	char facelets[128];
	// スレッド数
	//int num_threads = 2;
	// プログラム開始時間
	time_t tstart = (time_t)-1;
	// シード値
	unsigned long seed = (unsigned long)0;

	// 揃っているエッジ数
	int edges_fixed = -1;
	// EOしているエッジ数
	int edges_oriented = -1;
	// 揃っているコーナー数
	int corners_fixed = -1;
	// EOしているエッジ数
	int corners_oriented = -1;

	// コマンドライン引数の処理
	int i;
	facelets[0] = '\0';
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-n") == 0) {
			num_scrambles = atoi(argv[++i]);
			if (num_scrambles < 1 || 10000 < num_scrambles) {
				fprintf(stderr, "n must be between 1-10000.\n");
				exit(1);
			}
		} else if (strcmp(argv[i], "-t") == 0) {
			type = atoi(argv[++i]);
			if (type < 0 || 9 < type) {
				fprintf(stderr, "t must be between 0-9.\n");
				exit(1);
			}
		} else if (strcmp(argv[i], "-d") == 0) {
			depth = atoi(argv[++i]);
			if (depth < 0) {
				fprintf(stderr, "d must be positive.\n");
				exit(1);
			}
		} else if (strcmp(argv[i], "-l") == 0) {
			limit = (time_t)atoi(argv[++i]);
			if (limit < 0) {
				fprintf(stderr, "l must be positive.\n");
				exit(1);
			}
		}/* else if (args[i].equals("-s")) {
			useSeparator = true;
		}*/ else if (strcmp(argv[i], "-v") == 0) {
			is_verbose = 1;
		} else if (strcmp(argv[i], "-ns") == 0) {
			show_status = 0;
		} else if (strcmp(argv[i], "-f") == 0) {
			strcpy(facelets, argv[++i]);
		/*{ else if (args[i].equals("-th")) {
			numThreads = Integer.parseInt(args[++i]);
			if (numThreads < 1 || 4 < numThreads) {
				System.err.println("th must be between 1-4.");
				System.exit(1);
			}
		}*/
		} else if (strcmp(argv[i], "-sd") == 0) {
			seed = atoi(argv[++i]);
		} else if (strcmp(argv[i], "-ef") == 0) {
			edges_fixed = atoi(argv[++i]);
			if (edges_fixed < 0 || 12 < edges_fixed) {
				fprintf(stderr, "ef must be between 0-12.\n");
				exit(1);
			}
		} else if (strcmp(argv[i], "-eo") == 0) {
			edges_oriented = atoi(argv[++i]);
			if (edges_oriented < 0 || 12 < edges_oriented) {
				fprintf(stderr, "eo must be between 0-12.\n");
				exit(1);
			}
		} else if (strcmp(argv[i], "-cf") == 0) {
			corners_fixed = atoi(argv[++i]);
			if (corners_fixed < 0 || 8 < corners_fixed) {
				fprintf(stderr, "cf must be between 0-8.\n");
				exit(1);
			}
		} else if (strcmp(argv[i], "-co") == 0) {
			corners_oriented = atoi(argv[++i]);
			if (corners_oriented < 0 || 8 < corners_oriented) {
				fprintf(stderr, "co must be between 0-8.\n");
				exit(1);
			}
		} else {
			fprintf(stderr, "Bad arguments.\n");
			exit(1);
		}
	}

	// エラーとするオプションの組み合わせ
	if (type == 7) {
		if (edges_fixed < 0) {
			fprintf(stderr, "ef must be specified if type is 7.\n");
			exit(1);
		}
		if (edges_oriented < 0) {
			fprintf(stderr, "eo must be specified if type is 7.\n");
			exit(1);
		}
		if (corners_fixed < 0) {
			fprintf(stderr, "cf must be specified if type is 7.\n");
			exit(1);
		}
		if (corners_oriented < 0) {
			fprintf(stderr, "eo must be specified if type is 7.\n");
			exit(1);
		}
	}

	// プログラム名表示
	if (is_verbose) {
		printf("** TwophaseDriver in C 0.0.1 **\n");
		// 各パラメータ表示
		printf("num_scrambles=%d\n", num_scrambles);
		printf("type=%d\n", type);
		printf("depth=%d\n", depth);
		printf("limit=%ld\n", (long int)limit);
		printf("is_verbose=%d\n", is_verbose);
		printf("facelets=%s\n", facelets);
	}

	// 計測開始
	if (is_verbose) {
		tstart = clock();
	}

	// 静的データの初期化
	if (seed == 0) {
		mt_init((unsigned long)time(NULL));
	} else {
		mt_init(seed);
	}
	cubiecube_init();
	coordcube_init();
	if (facelets[0] != '\0') {
		num_scrambles = 1;
	}

	// スクランブル生成
	for (i = 0; i < num_scrambles; i++) {
		Search sc;
		char stat[128], result[128], output[256];

		//output[0] = '\0';
		// Facelet文字列指定されていたら
		if (facelets[0] != '\0') {
			strcpy(stat, facelets);
		}
		// スクランブルタイプによってスクランブルされたキューブの状態を生成する
		else {
			switch (type) {
				case 0: // 通常
					random_cube(stat);
					break;
				case 1: // コーナーのみ
					corner_random_cube(stat);
					break;
				case 2: // エッジのみ
					edge_random_cube(stat);
					break;
				case 3: // パリティ有り
					random_cube_with_parity(stat);
					break;
				case 4: // パリティ無し
					random_cube_with_no_parity(stat);
					break;
				case 5: // YY君が作成してくれたキューブ状態 (test 1)
					yy_cube_test1(stat);
					break;
				case 6: // YY君が作成してくれたキューブ状態 (test 2)
					yy_cube_test2(stat);
					break;
				case 7: // E-fixed, EO, C-fixed, CO を指定する (パリティランダム)
					fixed_cube(stat, edges_fixed, edges_oriented, corners_fixed, corners_oriented, 0);
					break;
				case 8: // E-fixed, EO, C-fixed, CO を指定する (パリティ無し)
					fixed_cube(stat, edges_fixed, edges_oriented, corners_fixed, corners_oriented, 1);
					break;
				case 9: // E-fixed, EO, C-fixed, CO を指定する (パリティ有り)
					fixed_cube(stat, edges_fixed, edges_oriented, corners_fixed, corners_oriented, 2);
					break;
			}
		}

		if (stat[0] == 'T') { // キューブ状態生成タイムアウト
			printf("Timeout\nTimeout\n");
		} else {
			/*if (is_verbose) {
				printf("[%d] CubeString:\n%s\n", i, stat);
				char cs[64];
				sprintf(cs, "\n[%d] CubeString\n", i);
				output = strcat(output, cs);

				output += "\n" + getName() + ": [" + i + "] CubeString\n";
				output += c.substring(0, 9) + " - ";
				output += c.substring(9, 18) + " - ";
				output += c.substring(18, 27) + " - ";
				output += c.substring(27, 36) + " - ";
				output += c.substring(36, 45) + " - ";
				output += c.substring(45, 54) + "\n";
			}*/
			// 状態を表示
			if (show_status) {
				printf("%s\n", stat);
			}

			// その状態から完成状態までの解法を検索
			// 参考: このメソッドの引数
			//      solution(java.lang.String facelets, int maxDepth, long timeOut, boolean useSeparator)
			//        Computes the solver string for a given cube.
			solution(&sc, stat, depth, limit, 0, result);
			/*if (is_verbose) {
				//output = strcat(output, );
				//output += getName() + ": [" + i + "] Result\n";
				printf("[%d] Result:\n%s\n", i, result);
			}*/
			if (result[0] == 'E') { // if Error
				strcpy(output, result);
			} else {
				reverse_alg(result, output);
			}
			printf("%s\n", output);
		}
	}

	// 実行時間の計算
	if (is_verbose) {
		printf("\n");
		printf("CPU Time (sec): %lf\n", (double)(clock() - tstart) / CLOCKS_PER_SEC);
	}

	return 0;
}
