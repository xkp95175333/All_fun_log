```asm

text:00000001401C901C CC CC CC CC                             align 20h
.text:00000001401C9020
.text:00000001401C9020                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9020
.text:00000001401C9020
.text:00000001401C9020                         sub_1401C9020   proc near
.text:00000001401C9020
.text:00000001401C9020                         arg_0           = byte ptr  8
.text:00000001401C9020
.text:00000001401C9020 40 53                                   push    rbx
.text:00000001401C9022 48 83 EC 20                             sub     rsp, 20h
.text:00000001401C9026 B9 10 00 00 00                          mov     ecx, 10h
.text:00000001401C902B E8 A0 3D 8F 02                          call    sub_142ABCDD0
.text:00000001401C9030 48 8B D8                                mov     rbx, rax
.text:00000001401C9033 48 85 C0                                test    rax, rax
.text:00000001401C9036 74 1A                                   jz      short loc_1401C9052
.text:00000001401C9038 C7 40 08 40 00 00 00                    mov     dword ptr [rax+8], 40h ; '@'
.text:00000001401C903F C7 40 0C 08 00 00 00                    mov     dword ptr [rax+0Ch], 8
.text:00000001401C9046 48 8D 05 03 52 53 10                    lea     rax, off_1506FE250
.text:00000001401C904D 48 89 03                                mov     [rbx], rax
.text:00000001401C9050 EB 02                                   jmp     short loc_1401C9054
.text:00000001401C9052                         ; ---------------------------------------------------------------------------
.text:00000001401C9052
.text:00000001401C9052                         loc_1401C9052:                          ; CODE XREF: sub_1401C9020+16↑j
.text:00000001401C9052 33 DB                                   xor     ebx, ebx
.text:00000001401C9054
.text:00000001401C9054                         loc_1401C9054:                          ; CODE XREF: sub_1401C9020+30↑j
.text:00000001401C9054 41 B8 01 00 00 00                       mov     r8d, 1
.text:00000001401C905A 48 8D 15 C7 3E 53 10                    lea     rdx, aPreloadassetde ; "PreloadAssetDescRowBase"
.text:00000001401C9061 48 8D 4C 24 30                          lea     rcx, [rsp+28h+arg_0]
.text:00000001401C9066 E8 C5 C3 70 0C                          call    sub_14C8D5430
.text:00000001401C906B 48 8B D3                                mov     rdx, rbx
.text:00000001401C906E 48 8B 08                                mov     rcx, [rax]
.text:00000001401C9071 E8 2A 7F 8C 0C                          call    sub_14CA90FA0
.text:00000001401C9076 48 83 C4 20                             add     rsp, 20h
.text:00000001401C907A 5B                                      pop     rbx
.text:00000001401C907B C3                                      retn
.text:00000001401C907B                         sub_1401C9020   endp
.text:00000001401C907B
.text:00000001401C907B                         ; ---------------------------------------------------------------------------
.text:00000001401C907C CC CC CC CC                             align 20h
.text:00000001401C9080 66 0F 6F 05 A8 E4 4F 10                 movdqa  xmm0, cs:xmmword_1506C7530
.text:00000001401C9088 66 0F 7F 05 E0 11 C1 16                 movdqa  cs:xmmword_156DDA270, xmm0
.text:00000001401C9090 C3                                      retn
.text:00000001401C9090                         ; ---------------------------------------------------------------------------
.text:00000001401C9091 CC CC CC CC CC CC CC CC…                align 20h
.text:00000001401C90A0
.text:00000001401C90A0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C90A0
.text:00000001401C90A0
.text:00000001401C90A0                         sub_1401C90A0   proc near
.text:00000001401C90A0
.text:00000001401C90A0                         var_18          = qword ptr -18h
.text:00000001401C90A0
.text:00000001401C90A0 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C90A4 45 33 C9                                xor     r9d, r9d
.text:00000001401C90A7 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C90A7 00
.text:00000001401C90B0 4C 8D 05 89 07 53 10                    lea     r8, aEappupdatetype ; "EAppUpdateType"
.text:00000001401C90B7 48 8D 15 62 38 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C90BE 48 8D 0D AB 59 9B 00                    lea     rcx, sub_140B7EA70
.text:00000001401C90C5 E8 86 A0 A8 0C                          call    sub_14CC53150
.text:00000001401C90CA 48 83 C4 38                             add     rsp, 38h
.text:00000001401C90CE C3                                      retn
.text:00000001401C90CE                         sub_1401C90A0   endp
.text:00000001401C90CE
.text:00000001401C90CE                         ; ---------------------------------------------------------------------------
.text:00000001401C90CF CC                                      align 10h
.text:00000001401C90D0
.text:00000001401C90D0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C90D0
.text:00000001401C90D0
.text:00000001401C90D0                         sub_1401C90D0   proc near
.text:00000001401C90D0
.text:00000001401C90D0                         var_18          = qword ptr -18h
.text:00000001401C90D0
.text:00000001401C90D0 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C90D4 45 33 C9                                xor     r9d, r9d
.text:00000001401C90D7 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C90D7 00
.text:00000001401C90E0 4C 8D 05 F9 2C 53 10                    lea     r8, aEdatatableload ; "EDataTableLoadingStep"
.text:00000001401C90E7 48 8D 15 32 38 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C90EE 48 8D 0D BB 59 9B 00                    lea     rcx, sub_140B7EAB0
.text:00000001401C90F5 E8 56 A0 A8 0C                          call    sub_14CC53150
.text:00000001401C90FA 48 83 C4 38                             add     rsp, 38h
.text:00000001401C90FE C3                                      retn
.text:00000001401C90FE                         sub_1401C90D0   endp
.text:00000001401C90FE
.text:00000001401C90FE                         ; ---------------------------------------------------------------------------
.text:00000001401C90FF CC                                      align 20h
.text:00000001401C9100
.text:00000001401C9100                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9100
.text:00000001401C9100
.text:00000001401C9100                         sub_1401C9100   proc near
.text:00000001401C9100
.text:00000001401C9100                         var_18          = qword ptr -18h
.text:00000001401C9100
.text:00000001401C9100 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C9104 45 33 C9                                xor     r9d, r9d
.text:00000001401C9107 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C9107 00
.text:00000001401C9110 4C 8D 05 E1 1D 53 10                    lea     r8, aEdatatablerepl ; "EDataTableReplaceScene"
.text:00000001401C9117 48 8D 15 02 38 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C911E 48 8D 0D CB 59 9B 00                    lea     rcx, sub_140B7EAF0
.text:00000001401C9125 E8 26 A0 A8 0C                          call    sub_14CC53150
.text:00000001401C912A 48 83 C4 38                             add     rsp, 38h
.text:00000001401C912E C3                                      retn
.text:00000001401C912E                         sub_1401C9100   endp
.text:00000001401C912E
.text:00000001401C912E                         ; ---------------------------------------------------------------------------
.text:00000001401C912F CC                                      align 10h
.text:00000001401C9130
.text:00000001401C9130                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9130
.text:00000001401C9130
.text:00000001401C9130                         sub_1401C9130   proc near
.text:00000001401C9130
.text:00000001401C9130                         var_18          = qword ptr -18h
.text:00000001401C9130
.text:00000001401C9130 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C9134 45 33 C9                                xor     r9d, r9d
.text:00000001401C9137 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C9137 00
.text:00000001401C9140 4C 8D 05 C9 2C 53 10                    lea     r8, aEdatatableusec ; "EDataTableUseClientType"
.text:00000001401C9147 48 8D 15 D2 37 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C914E 48 8D 0D DB 59 9B 00                    lea     rcx, sub_140B7EB30
.text:00000001401C9155 E8 F6 9F A8 0C                          call    sub_14CC53150
.text:00000001401C915A 48 83 C4 38                             add     rsp, 38h
.text:00000001401C915E C3                                      retn
.text:00000001401C915E                         sub_1401C9130   endp
.text:00000001401C915E
.text:00000001401C915E                         ; ---------------------------------------------------------------------------
.text:00000001401C915F CC                                      align 20h
.text:00000001401C9160
.text:00000001401C9160                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9160
.text:00000001401C9160
.text:00000001401C9160                         sub_1401C9160   proc near
.text:00000001401C9160
.text:00000001401C9160                         var_18          = qword ptr -18h
.text:00000001401C9160
.text:00000001401C9160 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C9164 45 33 C9                                xor     r9d, r9d
.text:00000001401C9167 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C9167 00
.text:00000001401C9170 4C 8D 05 91 2D 53 10                    lea     r8, aEdatatableuses ; "EDataTableUseScene"
.text:00000001401C9177 48 8D 15 A2 37 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C917E 48 8D 0D EB 59 9B 00                    lea     rcx, sub_140B7EB70
.text:00000001401C9185 E8 C6 9F A8 0C                          call    sub_14CC53150
.text:00000001401C918A 48 83 C4 38                             add     rsp, 38h
.text:00000001401C918E C3                                      retn
.text:00000001401C918E                         sub_1401C9160   endp
.text:00000001401C918E
.text:00000001401C918E                         ; ---------------------------------------------------------------------------
.text:00000001401C918F CC                                      align 10h
.text:00000001401C9190
.text:00000001401C9190                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9190
.text:00000001401C9190
.text:00000001401C9190                         sub_1401C9190   proc near
.text:00000001401C9190
.text:00000001401C9190                         var_18          = qword ptr -18h
.text:00000001401C9190
.text:00000001401C9190 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C9194 45 33 C9                                xor     r9d, r9d
.text:00000001401C9197 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C9197 00
.text:00000001401C91A0 4C 8D 05 61 3C 53 10                    lea     r8, aEpreloadtype ; "EPreloadType"
.text:00000001401C91A7 48 8D 15 72 37 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C91AE 48 8D 0D FB 59 9B 00                    lea     rcx, sub_140B7EBB0
.text:00000001401C91B5 E8 96 9F A8 0C                          call    sub_14CC53150
.text:00000001401C91BA 48 83 C4 38                             add     rsp, 38h
.text:00000001401C91BE C3                                      retn
.text:00000001401C91BE                         sub_1401C9190   endp
.text:00000001401C91BE
.text:00000001401C91BE                         ; ---------------------------------------------------------------------------
.text:00000001401C91BF CC                                      align 20h
.text:00000001401C91C0
.text:00000001401C91C0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C91C0
.text:00000001401C91C0
.text:00000001401C91C0                         sub_1401C91C0   proc near
.text:00000001401C91C0
.text:00000001401C91C0                         var_18          = qword ptr -18h
.text:00000001401C91C0
.text:00000001401C91C0 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C91C4 45 33 C9                                xor     r9d, r9d
.text:00000001401C91C7 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C91C7 00
.text:00000001401C91D0 4C 8D 05 59 2B 53 10                    lea     r8, aAppsettingform ; "AppSettingForMapleChannel"
.text:00000001401C91D7 48 8D 15 42 37 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C91DE 48 8D 0D 6B 5E 9B 00                    lea     rcx, sub_140B7F050
.text:00000001401C91E5 E8 96 A0 A8 0C                          call    sub_14CC53280
.text:00000001401C91EA 48 83 C4 38                             add     rsp, 38h
.text:00000001401C91EE C3                                      retn
.text:00000001401C91EE                         sub_1401C91C0   endp
.text:00000001401C91EE
.text:00000001401C91EE                         ; ---------------------------------------------------------------------------
.text:00000001401C91EF CC                                      align 10h
.text:00000001401C91F0
.text:00000001401C91F0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C91F0
.text:00000001401C91F0
.text:00000001401C91F0                         sub_1401C91F0   proc near
.text:00000001401C91F0
.text:00000001401C91F0                         var_18          = qword ptr -18h
.text:00000001401C91F0
.text:00000001401C91F0 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C91F4 45 33 C9                                xor     r9d, r9d
.text:00000001401C91F7 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C91F7 00
.text:00000001401C9200 4C 8D 05 81 2E 53 10                    lea     r8, aAppsettingforu ; "AppSettingForUpdate"
.text:00000001401C9207 48 8D 15 12 37 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C920E 48 8D 0D 9B 5E 9B 00                    lea     rcx, sub_140B7F0B0
.text:00000001401C9215 E8 66 A0 A8 0C                          call    sub_14CC53280
.text:00000001401C921A 48 83 C4 38                             add     rsp, 38h
.text:00000001401C921E C3                                      retn
.text:00000001401C921E                         sub_1401C91F0   endp
.text:00000001401C921E
.text:00000001401C921E                         ; ---------------------------------------------------------------------------
.text:00000001401C921F CC                                      align 20h
.text:00000001401C9220
.text:00000001401C9220                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9220
.text:00000001401C9220
.text:00000001401C9220                         sub_1401C9220   proc near
.text:00000001401C9220
.text:00000001401C9220                         var_18          = qword ptr -18h
.text:00000001401C9220
.text:00000001401C9220 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C9224 45 33 C9                                xor     r9d, r9d
.text:00000001401C9227 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C9227 00
.text:00000001401C9230 4C 8D 05 71 30 53 10                    lea     r8, aAppsettingforu_0 ; "AppSettingForUpdateChannel"
.text:00000001401C9237 48 8D 15 E2 36 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C923E 48 8D 0D CB 5E 9B 00                    lea     rcx, sub_140B7F110
.text:00000001401C9245 E8 36 A0 A8 0C                          call    sub_14CC53280
.text:00000001401C924A 48 83 C4 38                             add     rsp, 38h
.text:00000001401C924E C3                                      retn
.text:00000001401C924E                         sub_1401C9220   endp
.text:00000001401C924E
.text:00000001401C924E                         ; ---------------------------------------------------------------------------
.text:00000001401C924F CC                                      align 10h
.text:00000001401C9250
.text:00000001401C9250                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9250
.text:00000001401C9250
.text:00000001401C9250                         sub_1401C9250   proc near
.text:00000001401C9250
.text:00000001401C9250                         var_18          = qword ptr -18h
.text:00000001401C9250
.text:00000001401C9250 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C9254 45 33 C9                                xor     r9d, r9d
.text:00000001401C9257 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C9257 00
.text:00000001401C9260 4C 8D 05 F9 05 53 10                    lea     r8, aAppsettingrow ; "AppSettingRow"
.text:00000001401C9267 48 8D 15 B2 36 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C926E 48 8D 0D FB 5E 9B 00                    lea     rcx, sub_140B7F170
.text:00000001401C9275 E8 06 A0 A8 0C                          call    sub_14CC53280
.text:00000001401C927A 48 83 C4 38                             add     rsp, 38h
.text:00000001401C927E C3                                      retn
.text:00000001401C927E                         sub_1401C9250   endp
.text:00000001401C927E
.text:00000001401C927E                         ; ---------------------------------------------------------------------------
.text:00000001401C927F CC                                      align 20h
.text:00000001401C9280
.text:00000001401C9280                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9280
.text:00000001401C9280
.text:00000001401C9280                         sub_1401C9280   proc near
.text:00000001401C9280
.text:00000001401C9280                         var_18          = qword ptr -18h
.text:00000001401C9280
.text:00000001401C9280 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C9284 45 33 C9                                xor     r9d, r9d
.text:00000001401C9287 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C9287 00
.text:00000001401C9290 4C 8D 05 49 3D 53 10                    lea     r8, aAssetdescrowba ; "AssetDescRowBase"
.text:00000001401C9297 48 8D 15 82 36 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C929E 48 8D 0D 2B 5F 9B 00                    lea     rcx, sub_140B7F1D0
.text:00000001401C92A5 E8 D6 9F A8 0C                          call    sub_14CC53280
.text:00000001401C92AA 48 83 C4 38                             add     rsp, 38h
.text:00000001401C92AE C3                                      retn
.text:00000001401C92AE                         sub_1401C9280   endp
.text:00000001401C92AE
.text:00000001401C92AE                         ; ---------------------------------------------------------------------------
.text:00000001401C92AF CC                                      align 10h
.text:00000001401C92B0
.text:00000001401C92B0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C92B0
.text:00000001401C92B0
.text:00000001401C92B0                         sub_1401C92B0   proc near
.text:00000001401C92B0
.text:00000001401C92B0                         var_18          = qword ptr -18h
.text:00000001401C92B0
.text:00000001401C92B0 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C92B4 45 33 C9                                xor     r9d, r9d
.text:00000001401C92B7 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C92B7 00
.text:00000001401C92C0 4C 8D 05 C9 39 53 10                    lea     r8, aAssetretargeti ; "AssetRetargetInfoRow"
.text:00000001401C92C7 48 8D 15 52 36 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C92CE 48 8D 0D 5B 5F 9B 00                    lea     rcx, sub_140B7F230
.text:00000001401C92D5 E8 A6 9F A8 0C                          call    sub_14CC53280
.text:00000001401C92DA 48 83 C4 38                             add     rsp, 38h
.text:00000001401C92DE C3                                      retn
.text:00000001401C92DE                         sub_1401C92B0   endp
.text:00000001401C92DE
.text:00000001401C92DE                         ; ---------------------------------------------------------------------------
.text:00000001401C92DF CC                                      align 20h
.text:00000001401C92E0
.text:00000001401C92E0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C92E0
.text:00000001401C92E0
.text:00000001401C92E0                         sub_1401C92E0   proc near
.text:00000001401C92E0
.text:00000001401C92E0                         var_18          = qword ptr -18h
.text:00000001401C92E0
.text:00000001401C92E0 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C92E4 45 33 C9                                xor     r9d, r9d
.text:00000001401C92E7 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C92E7 00
.text:00000001401C92F0 4C 8D 05 D1 3D 53 10                    lea     r8, aAssetstoreitem ; "AssetStoreItemRow"
.text:00000001401C92F7 48 8D 15 22 36 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C92FE 48 8D 0D 8B 5F 9B 00                    lea     rcx, sub_140B7F290
.text:00000001401C9305 E8 76 9F A8 0C                          call    sub_14CC53280
.text:00000001401C930A 48 83 C4 38                             add     rsp, 38h
.text:00000001401C930E C3                                      retn
.text:00000001401C930E                         sub_1401C92E0   endp
.text:00000001401C930E
.text:00000001401C930E                         ; ---------------------------------------------------------------------------
.text:00000001401C930F CC                                      align 10h
.text:00000001401C9310
.text:00000001401C9310                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9310
.text:00000001401C9310
.text:00000001401C9310                         sub_1401C9310   proc near
.text:00000001401C9310
.text:00000001401C9310                         var_18          = qword ptr -18h
.text:00000001401C9310
.text:00000001401C9310 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C9314 45 33 C9                                xor     r9d, r9d
.text:00000001401C9317 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C9317 00
.text:00000001401C9320 4C 8D 05 91 37 53 10                    lea     r8, aAsyncreqgroup ; "AsyncReqGroup"
.text:00000001401C9327 48 8D 15 F2 35 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C932E 48 8D 0D BB 5F 9B 00                    lea     rcx, sub_140B7F2F0
.text:00000001401C9335 E8 46 9F A8 0C                          call    sub_14CC53280
.text:00000001401C933A 48 83 C4 38                             add     rsp, 38h
.text:00000001401C933E C3                                      retn
.text:00000001401C933E                         sub_1401C9310   endp
.text:00000001401C933E
.text:00000001401C933E                         ; ---------------------------------------------------------------------------
.text:00000001401C933F CC                                      align 20h
.text:00000001401C9340
.text:00000001401C9340                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9340
.text:00000001401C9340
.text:00000001401C9340                         sub_1401C9340   proc near
.text:00000001401C9340
.text:00000001401C9340                         var_18          = qword ptr -18h
.text:00000001401C9340
.text:00000001401C9340 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C9344 45 33 C9                                xor     r9d, r9d
.text:00000001401C9347 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C9347 00
.text:00000001401C9350 4C 8D 05 91 37 53 10                    lea     r8, aAsyncreqstruct ; "AsyncReqStruct"
.text:00000001401C9357 48 8D 15 C2 35 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C935E 48 8D 0D EB 5F 9B 00                    lea     rcx, sub_140B7F350
.text:00000001401C9365 E8 16 9F A8 0C                          call    sub_14CC53280
.text:00000001401C936A 48 83 C4 38                             add     rsp, 38h
.text:00000001401C936E C3                                      retn
.text:00000001401C936E                         sub_1401C9340   endp
.text:00000001401C936E
.text:00000001401C936E                         ; ---------------------------------------------------------------------------
.text:00000001401C936F CC                                      align 10h
.text:00000001401C9370
.text:00000001401C9370                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9370
.text:00000001401C9370
.text:00000001401C9370                         sub_1401C9370   proc near
.text:00000001401C9370
.text:00000001401C9370                         var_18          = qword ptr -18h
.text:00000001401C9370
.text:00000001401C9370 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C9374 45 33 C9                                xor     r9d, r9d
.text:00000001401C9377 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C9377 00
.text:00000001401C9380 4C 8D 05 81 1C 53 10                    lea     r8, aCsv2datatable_0 ; "CSV2Datatable"
.text:00000001401C9387 48 8D 15 92 35 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C938E 48 8D 0D 1B 60 9B 00                    lea     rcx, sub_140B7F3B0
.text:00000001401C9395 E8 E6 9E A8 0C                          call    sub_14CC53280
.text:00000001401C939A 48 83 C4 38                             add     rsp, 38h
.text:00000001401C939E C3                                      retn
.text:00000001401C939E                         sub_1401C9370   endp
.text:00000001401C939E
.text:00000001401C939E                         ; ---------------------------------------------------------------------------
.text:00000001401C939F CC                                      align 20h
.text:00000001401C93A0
.text:00000001401C93A0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C93A0
.text:00000001401C93A0
.text:00000001401C93A0                         sub_1401C93A0   proc near
.text:00000001401C93A0
.text:00000001401C93A0                         var_18          = qword ptr -18h
.text:00000001401C93A0
.text:00000001401C93A0 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C93A4 45 33 C9                                xor     r9d, r9d
.text:00000001401C93A7 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C93A7 00
.text:00000001401C93B0 4C 8D 05 41 1E 53 10                    lea     r8, aDatatableasset ; "DataTableAssetInfo"
.text:00000001401C93B7 48 8D 15 62 35 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C93BE 48 8D 0D 4B 60 9B 00                    lea     rcx, sub_140B7F410
.text:00000001401C93C5 E8 B6 9E A8 0C                          call    sub_14CC53280
.text:00000001401C93CA 48 83 C4 38                             add     rsp, 38h
.text:00000001401C93CE C3                                      retn
.text:00000001401C93CE                         sub_1401C93A0   endp
.text:00000001401C93CE
.text:00000001401C93CE                         ; ---------------------------------------------------------------------------
.text:00000001401C93CF CC                                      align 10h
.text:00000001401C93D0
.text:00000001401C93D0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C93D0
.text:00000001401C93D0
.text:00000001401C93D0                         sub_1401C93D0   proc near
.text:00000001401C93D0
.text:00000001401C93D0                         var_18          = qword ptr -18h
.text:00000001401C93D0
.text:00000001401C93D0 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C93D4 45 33 C9                                xor     r9d, r9d
.text:00000001401C93D7 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C93D7 00
.text:00000001401C93E0 4C 8D 05 41 2C 53 10                    lea     r8, aDatatableusesc ; "DataTableUseSceneValue"
.text:00000001401C93E7 48 8D 15 32 35 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C93EE 48 8D 0D 7B 60 9B 00                    lea     rcx, sub_140B7F470
.text:00000001401C93F5 E8 86 9E A8 0C                          call    sub_14CC53280
.text:00000001401C93FA 48 83 C4 38                             add     rsp, 38h
.text:00000001401C93FE C3                                      retn
.text:00000001401C93FE                         sub_1401C93D0   endp
.text:00000001401C93FE
.text:00000001401C93FE                         ; ---------------------------------------------------------------------------
.text:00000001401C93FF CC                                      align 20h
.text:00000001401C9400
.text:00000001401C9400                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9400
.text:00000001401C9400
.text:00000001401C9400                         sub_1401C9400   proc near
.text:00000001401C9400
.text:00000001401C9400                         var_18          = qword ptr -18h
.text:00000001401C9400
.text:00000001401C9400 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C9404 45 33 C9                                xor     r9d, r9d
.text:00000001401C9407 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C9407 00
.text:00000001401C9410 4C 8D 05 F1 0F 53 10                    lea     r8, aGametypeconfig ; "GameTypeConfig"
.text:00000001401C9417 48 8D 15 02 35 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C941E 48 8D 0D AB 60 9B 00                    lea     rcx, sub_140B7F4D0
.text:00000001401C9425 E8 56 9E A8 0C                          call    sub_14CC53280
.text:00000001401C942A 48 83 C4 38                             add     rsp, 38h
.text:00000001401C942E C3                                      retn
.text:00000001401C942E                         sub_1401C9400   endp
.text:00000001401C942E
.text:00000001401C942E                         ; ---------------------------------------------------------------------------
.text:00000001401C942F CC                                      align 10h
.text:00000001401C9430
.text:00000001401C9430                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9430
.text:00000001401C9430
.text:00000001401C9430                         sub_1401C9430   proc near
.text:00000001401C9430
.text:00000001401C9430                         var_18          = qword ptr -18h
.text:00000001401C9430
.text:00000001401C9430 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C9434 45 33 C9                                xor     r9d, r9d
.text:00000001401C9437 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C9437 00
.text:00000001401C9440 4C 8D 05 E9 1D 53 10                    lea     r8, aLoginsettingro ; "LoginSettingRow"
.text:00000001401C9447 48 8D 15 D2 34 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C944E 48 8D 0D DB 60 9B 00                    lea     rcx, sub_140B7F530
.text:00000001401C9455 E8 26 9E A8 0C                          call    sub_14CC53280
.text:00000001401C945A 48 83 C4 38                             add     rsp, 38h
.text:00000001401C945E C3                                      retn
.text:00000001401C945E                         sub_1401C9430   endp
.text:00000001401C945E
.text:00000001401C945E                         ; ---------------------------------------------------------------------------
.text:00000001401C945F CC                                      align 20h
.text:00000001401C9460
.text:00000001401C9460                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9460
.text:00000001401C9460
.text:00000001401C9460                         sub_1401C9460   proc near
.text:00000001401C9460
.text:00000001401C9460                         var_18          = qword ptr -18h
.text:00000001401C9460
.text:00000001401C9460 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C9464 45 33 C9                                xor     r9d, r9d
.text:00000001401C9467 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C9467 00
.text:00000001401C9470 4C 8D 05 A1 24 53 10                    lea     r8, aMaplesettingro ; "MapleSettingRow"
.text:00000001401C9477 48 8D 15 A2 34 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C947E 48 8D 0D 0B 61 9B 00                    lea     rcx, sub_140B7F590
.text:00000001401C9485 E8 F6 9D A8 0C                          call    sub_14CC53280
.text:00000001401C948A 48 83 C4 38                             add     rsp, 38h
.text:00000001401C948E C3                                      retn
.text:00000001401C948E                         sub_1401C9460   endp
.text:00000001401C948E
.text:00000001401C948E                         ; ---------------------------------------------------------------------------
.text:00000001401C948F CC                                      align 10h
.text:00000001401C9490
.text:00000001401C9490                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9490
.text:00000001401C9490
.text:00000001401C9490                         sub_1401C9490   proc near
.text:00000001401C9490
.text:00000001401C9490                         var_18          = qword ptr -18h
.text:00000001401C9490
.text:00000001401C9490 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C9494 45 33 C9                                xor     r9d, r9d
.text:00000001401C9497 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C9497 00
.text:00000001401C94A0 4C 8D 05 81 39 53 10                    lea     r8, aPackagedescrow ; "PackageDescRow"
.text:00000001401C94A7 48 8D 15 72 34 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C94AE 48 8D 0D 3B 61 9B 00                    lea     rcx, sub_140B7F5F0
.text:00000001401C94B5 E8 C6 9D A8 0C                          call    sub_14CC53280
.text:00000001401C94BA 48 83 C4 38                             add     rsp, 38h
.text:00000001401C94BE C3                                      retn
.text:00000001401C94BE                         sub_1401C9490   endp
.text:00000001401C94BE
.text:00000001401C94BE                         ; ---------------------------------------------------------------------------
.text:00000001401C94BF CC                                      align 20h
.text:00000001401C94C0
.text:00000001401C94C0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C94C0
.text:00000001401C94C0
.text:00000001401C94C0                         sub_1401C94C0   proc near
.text:00000001401C94C0
.text:00000001401C94C0                         var_18          = qword ptr -18h
.text:00000001401C94C0
.text:00000001401C94C0 48 83 EC 38                             sub     rsp, 38h
.text:00000001401C94C4 45 33 C9                                xor     r9d, r9d
.text:00000001401C94C7 48 C7 44 24 20 00 00 00                 mov     [rsp+38h+var_18], 0
.text:00000001401C94C7 00
.text:00000001401C94D0 4C 8D 05 51 3A 53 10                    lea     r8, aPreloadassetde ; "PreloadAssetDescRowBase"
.text:00000001401C94D7 48 8D 15 42 34 53 10                    lea     rdx, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C94DE 48 8D 0D 6B 61 9B 00                    lea     rcx, sub_140B7F650
.text:00000001401C94E5 E8 96 9D A8 0C                          call    sub_14CC53280
.text:00000001401C94EA 48 83 C4 38                             add     rsp, 38h
.text:00000001401C94EE C3                                      retn
.text:00000001401C94EE                         sub_1401C94C0   endp
.text:00000001401C94EE
.text:00000001401C94EE                         ; ---------------------------------------------------------------------------
.text:00000001401C94EF CC                                      align 10h
.text:00000001401C94F0
.text:00000001401C94F0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C94F0
.text:00000001401C94F0
.text:00000001401C94F0                         sub_1401C94F0   proc near
.text:00000001401C94F0
.text:00000001401C94F0                         var_28          = byte ptr -28h
.text:00000001401C94F0                         var_20          = qword ptr -20h
.text:00000001401C94F0                         var_18          = qword ptr -18h
.text:00000001401C94F0
.text:00000001401C94F0 48 83 EC 48                             sub     rsp, 48h
.text:00000001401C94F4 33 C0                                   xor     eax, eax
.text:00000001401C94F6 4C 8D 0D 23 34 53 10                    lea     r9, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C94FD 48 89 44 24 30                          mov     [rsp+48h+var_18], rax
.text:00000001401C9502 4C 8D 05 A7 1B 53 10                    lea     r8, aUanimnotifyPla ; "UAnimNotify_PlayParticleEffectGroup"
.text:00000001401C9509 48 89 44 24 28                          mov     [rsp+48h+var_20], rax
.text:00000001401C950E 48 8D 15 FB 32 9B 00                    lea     rdx, sub_140B7C810
.text:00000001401C9515 48 8D 0D 94 61 9B 00                    lea     rcx, sub_140B7F6B0
.text:00000001401C951C 88 44 24 20                             mov     [rsp+48h+var_28], al
.text:00000001401C9520 E8 7B 93 A8 0C                          call    sub_14CC528A0
.text:00000001401C9525 48 83 C4 48                             add     rsp, 48h
.text:00000001401C9529 C3                                      retn
.text:00000001401C9529                         sub_1401C94F0   endp
.text:00000001401C9529
.text:00000001401C9529                         ; ---------------------------------------------------------------------------
.text:00000001401C952A CC CC CC CC CC CC                       align 10h
.text:00000001401C9530
.text:00000001401C9530                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9530
.text:00000001401C9530
.text:00000001401C9530                         sub_1401C9530   proc near
.text:00000001401C9530
.text:00000001401C9530                         var_28          = byte ptr -28h
.text:00000001401C9530                         var_20          = qword ptr -20h
.text:00000001401C9530                         var_18          = qword ptr -18h
.text:00000001401C9530
.text:00000001401C9530 48 83 EC 48                             sub     rsp, 48h
.text:00000001401C9534 33 C0                                   xor     eax, eax
.text:00000001401C9536 4C 8D 0D E3 33 53 10                    lea     r9, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C953D 48 89 44 24 30                          mov     [rsp+48h+var_18], rax
.text:00000001401C9542 4C 8D 05 BF 33 53 10                    lea     r8, aUappsetting ; "UAppSetting"
.text:00000001401C9549 48 89 44 24 28                          mov     [rsp+48h+var_20], rax
.text:00000001401C954E 48 8D 15 4B 3D 9B 00                    lea     rdx, sub_140B7D2A0
.text:00000001401C9555 48 8D 0D 84 2F 9B 00                    lea     rcx, sub_140B7C4E0
.text:00000001401C955C 88 44 24 20                             mov     [rsp+48h+var_28], al
.text:00000001401C9560 E8 3B 93 A8 0C                          call    sub_14CC528A0
.text:00000001401C9565 48 83 C4 48                             add     rsp, 48h
.text:00000001401C9569 C3                                      retn
.text:00000001401C9569                         sub_1401C9530   endp
.text:00000001401C9569
.text:00000001401C9569                         ; ---------------------------------------------------------------------------
.text:00000001401C956A CC CC CC CC CC CC                       align 10h
.text:00000001401C9570
.text:00000001401C9570                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9570
.text:00000001401C9570
.text:00000001401C9570                         sub_1401C9570   proc near
.text:00000001401C9570
.text:00000001401C9570                         var_28          = byte ptr -28h
.text:00000001401C9570                         var_20          = qword ptr -20h
.text:00000001401C9570                         var_18          = qword ptr -18h
.text:00000001401C9570
.text:00000001401C9570 48 83 EC 48                             sub     rsp, 48h
.text:00000001401C9574 33 C0                                   xor     eax, eax
.text:00000001401C9576 4C 8D 0D A3 33 53 10                    lea     r9, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C957D 48 89 44 24 30                          mov     [rsp+48h+var_18], rax
.text:00000001401C9582 4C 8D 05 07 35 53 10                    lea     r8, aUappsettingsex ; "UAppSettingsExtend"
.text:00000001401C9589 48 89 44 24 28                          mov     [rsp+48h+var_20], rax
.text:00000001401C958E 48 8D 15 7B 37 9B 00                    lea     rdx, sub_140B7CD10
.text:00000001401C9595 48 8D 0D 44 61 9B 00                    lea     rcx, sub_140B7F6E0
.text:00000001401C959C 88 44 24 20                             mov     [rsp+48h+var_28], al
.text:00000001401C95A0 E8 FB 92 A8 0C                          call    sub_14CC528A0
.text:00000001401C95A5 48 83 C4 48                             add     rsp, 48h
.text:00000001401C95A9 C3                                      retn
.text:00000001401C95A9                         sub_1401C9570   endp
.text:00000001401C95A9
.text:00000001401C95A9                         ; ---------------------------------------------------------------------------
.text:00000001401C95AA CC CC CC CC CC CC                       align 10h
.text:00000001401C95B0
.text:00000001401C95B0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C95B0
.text:00000001401C95B0
.text:00000001401C95B0                         sub_1401C95B0   proc near
.text:00000001401C95B0
.text:00000001401C95B0                         var_28          = byte ptr -28h
.text:00000001401C95B0                         var_20          = qword ptr -20h
.text:00000001401C95B0                         var_18          = qword ptr -18h
.text:00000001401C95B0
.text:00000001401C95B0 48 83 EC 48                             sub     rsp, 48h
.text:00000001401C95B4 33 C0                                   xor     eax, eax
.text:00000001401C95B6 4C 8D 0D 63 33 53 10                    lea     r9, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C95BD 48 89 44 24 30                          mov     [rsp+48h+var_18], rax
.text:00000001401C95C2 4C 8D 05 9F 36 53 10                    lea     r8, aUassetbasemana_1 ; "UAssetBaseManager"
.text:00000001401C95C9 48 89 44 24 28                          mov     [rsp+48h+var_20], rax
.text:00000001401C95CE 48 8D 15 EB 37 9B 00                    lea     rdx, sub_140B7CDC0
.text:00000001401C95D5 48 8D 0D 34 61 9B 00                    lea     rcx, sub_140B7F710
.text:00000001401C95DC 88 44 24 20                             mov     [rsp+48h+var_28], al
.text:00000001401C95E0 E8 BB 92 A8 0C                          call    sub_14CC528A0
.text:00000001401C95E5 48 83 C4 48                             add     rsp, 48h
.text:00000001401C95E9 C3                                      retn
.text:00000001401C95E9                         sub_1401C95B0   endp
.text:00000001401C95E9
.text:00000001401C95E9                         ; ---------------------------------------------------------------------------
.text:00000001401C95EA CC CC CC CC CC CC                       align 10h
.text:00000001401C95F0
.text:00000001401C95F0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C95F0
.text:00000001401C95F0
.text:00000001401C95F0                         sub_1401C95F0   proc near
.text:00000001401C95F0
.text:00000001401C95F0                         var_28          = byte ptr -28h
.text:00000001401C95F0                         var_20          = qword ptr -20h
.text:00000001401C95F0                         var_18          = qword ptr -18h
.text:00000001401C95F0
.text:00000001401C95F0 48 83 EC 48                             sub     rsp, 48h
.text:00000001401C95F4 33 C0                                   xor     eax, eax
.text:00000001401C95F6 4C 8D 0D 23 33 53 10                    lea     r9, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C95FD 48 89 44 24 30                          mov     [rsp+48h+var_18], rax
.text:00000001401C9602 4C 8D 05 C7 37 53 10                    lea     r8, aUassetbasemana_2 ; "UAssetBaseManagerSetting"
.text:00000001401C9609 48 89 44 24 28                          mov     [rsp+48h+var_20], rax
.text:00000001401C960E 48 8D 15 3B 36 9B 00                    lea     rdx, sub_140B7CC50
.text:00000001401C9615 48 8D 0D 24 61 9B 00                    lea     rcx, sub_140B7F740
.text:00000001401C961C 88 44 24 20                             mov     [rsp+48h+var_28], al
.text:00000001401C9620 E8 7B 92 A8 0C                          call    sub_14CC528A0
.text:00000001401C9625 48 83 C4 48                             add     rsp, 48h
.text:00000001401C9629 C3                                      retn
.text:00000001401C9629                         sub_1401C95F0   endp
.text:00000001401C9629
.text:00000001401C9629                         ; ---------------------------------------------------------------------------
.text:00000001401C962A CC CC CC CC CC CC                       align 10h
.text:00000001401C9630
.text:00000001401C9630                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9630
.text:00000001401C9630
.text:00000001401C9630                         sub_1401C9630   proc near
.text:00000001401C9630
.text:00000001401C9630                         var_28          = byte ptr -28h
.text:00000001401C9630                         var_20          = qword ptr -20h
.text:00000001401C9630                         var_18          = qword ptr -18h
.text:00000001401C9630
.text:00000001401C9630 48 83 EC 48                             sub     rsp, 48h
.text:00000001401C9634 33 C0                                   xor     eax, eax
.text:00000001401C9636 4C 8D 0D E3 32 53 10                    lea     r9, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C963D 48 89 44 24 30                          mov     [rsp+48h+var_18], rax
.text:00000001401C9642 4C 8D 05 DF 01 53 10                    lea     r8, aUassetstore ; "UAssetStore"
.text:00000001401C9649 48 89 44 24 28                          mov     [rsp+48h+var_20], rax
.text:00000001401C964E 48 8D 15 9B 3B 9B 00                    lea     rdx, sub_140B7D1F0
.text:00000001401C9655 48 8D 0D 14 61 9B 00                    lea     rcx, sub_140B7F770
.text:00000001401C965C 88 44 24 20                             mov     [rsp+48h+var_28], al
.text:00000001401C9660 E8 3B 92 A8 0C                          call    sub_14CC528A0
.text:00000001401C9665 48 83 C4 48                             add     rsp, 48h
.text:00000001401C9669 C3                                      retn
.text:00000001401C9669                         sub_1401C9630   endp
.text:00000001401C9669
.text:00000001401C9669                         ; ---------------------------------------------------------------------------
.text:00000001401C966A CC CC CC CC CC CC                       align 10h
.text:00000001401C9670
.text:00000001401C9670                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9670
.text:00000001401C9670
.text:00000001401C9670                         sub_1401C9670   proc near
.text:00000001401C9670
.text:00000001401C9670                         var_28          = byte ptr -28h
.text:00000001401C9670                         var_20          = qword ptr -20h
.text:00000001401C9670                         var_18          = qword ptr -18h
.text:00000001401C9670
.text:00000001401C9670 48 83 EC 48                             sub     rsp, 48h
.text:00000001401C9674 33 C0                                   xor     eax, eax
.text:00000001401C9676 4C 8D 0D A3 32 53 10                    lea     r9, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C967D 48 89 44 24 30                          mov     [rsp+48h+var_18], rax
.text:00000001401C9682 4C 8D 05 7F 1A 53 10                    lea     r8, aUbasegameinsta ; "UBaseGameInstance"
.text:00000001401C9689 48 89 44 24 28                          mov     [rsp+48h+var_20], rax
.text:00000001401C968E 48 8D 15 5B 30 9B 00                    lea     rdx, sub_140B7C6F0
.text:00000001401C9695 48 8D 0D 04 61 9B 00                    lea     rcx, sub_140B7F7A0
.text:00000001401C969C 88 44 24 20                             mov     [rsp+48h+var_28], al
.text:00000001401C96A0 E8 FB 91 A8 0C                          call    sub_14CC528A0
.text:00000001401C96A5 48 83 C4 48                             add     rsp, 48h
.text:00000001401C96A9 C3                                      retn
.text:00000001401C96A9                         sub_1401C9670   endp
.text:00000001401C96A9
.text:00000001401C96A9                         ; ---------------------------------------------------------------------------
.text:00000001401C96AA CC CC CC CC CC CC                       align 10h
.text:00000001401C96B0
.text:00000001401C96B0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C96B0
.text:00000001401C96B0
.text:00000001401C96B0                         sub_1401C96B0   proc near
.text:00000001401C96B0
.text:00000001401C96B0                         var_28          = byte ptr -28h
.text:00000001401C96B0                         var_20          = qword ptr -20h
.text:00000001401C96B0                         var_18          = qword ptr -18h
.text:00000001401C96B0
.text:00000001401C96B0 48 83 EC 48                             sub     rsp, 48h
.text:00000001401C96B4 33 C0                                   xor     eax, eax
.text:00000001401C96B6 4C 8D 0D 63 32 53 10                    lea     r9, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C96BD 48 89 44 24 30                          mov     [rsp+48h+var_18], rax
.text:00000001401C96C2 4C 8D 05 37 26 53 10                    lea     r8, aUcsv2datatable ; "UCSV2DatatableManager"
.text:00000001401C96C9 48 89 44 24 28                          mov     [rsp+48h+var_20], rax
.text:00000001401C96CE 48 8D 15 5B 3A 9B 00                    lea     rdx, sub_140B7D130
.text:00000001401C96D5 48 8D 0D F4 60 9B 00                    lea     rcx, sub_140B7F7D0
.text:00000001401C96DC 88 44 24 20                             mov     [rsp+48h+var_28], al
.text:00000001401C96E0 E8 BB 91 A8 0C                          call    sub_14CC528A0
.text:00000001401C96E5 48 83 C4 48                             add     rsp, 48h
.text:00000001401C96E9 C3                                      retn
.text:00000001401C96E9                         sub_1401C96B0   endp
.text:00000001401C96E9
.text:00000001401C96E9                         ; ---------------------------------------------------------------------------
.text:00000001401C96EA CC CC CC CC CC CC                       align 10h
.text:00000001401C96F0
.text:00000001401C96F0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C96F0
.text:00000001401C96F0
.text:00000001401C96F0                         sub_1401C96F0   proc near
.text:00000001401C96F0
.text:00000001401C96F0                         var_28          = byte ptr -28h
.text:00000001401C96F0                         var_20          = qword ptr -20h
.text:00000001401C96F0                         var_18          = qword ptr -18h
.text:00000001401C96F0
.text:00000001401C96F0 48 83 EC 48                             sub     rsp, 48h
.text:00000001401C96F4 33 C0                                   xor     eax, eax
.text:00000001401C96F6 4C 8D 0D 23 32 53 10                    lea     r9, aScriptGamecore ; "/Script/GameCore"
.text:00000001401C96FD 48 89 44 24 30                          mov     [rsp+48h+var_18], rax
.text:00000001401C9702 4C 8D 05 3F 23 53 10                    lea     r8, aUdatatableasse_1 ; "UDataTableAssetInfoManager"
.text:00000001401C9709 48 89 44 24 28                          mov     [rsp+48h+var_20], rax
.text:00000001401C970E 48 8D 15 2B 39 9B 00                    lea     rdx, sub_140B7D040
.text:00000001401C9715 48 8D 0D E4 60 9B 00                    lea     rcx, sub_140B7F800
.text:00000001401C971C 88 44 24 20                             mov     [rsp+48h+var_28], al
.text:00000001401C9720 E8 7B 91 A8 0C                          call    sub_14CC528A0
.text:00000001401C9725 48 83 C4 48                             add     rsp, 48h
.text:00000001401C9729 C3                                      retn
.text:00000001401C9729                         sub_1401C96F0   endp
.text:00000001401C9729
.text:00000001401C9729                         ; ---------------------------------------------------------------------------
.text:00000001401C972A CC CC CC CC CC CC                       align 10h
.text:00000001401C9730
.text:00000001401C9730                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9730
.text:00000001401C9730
.text:00000001401C9730                         sub_1401C9730   proc near
.text:00000001401C9730 41 B9 42 F9 43 05                       mov     r9d, 543F942h
.text:00000001401C9736 48 8D 15 93 88 53 10                    lea     rdx, aUdatatablesyst_11 ; "UDataTableSystemImpl"
.text:00000001401C973D 41 B8 A0 00 00 00                       mov     r8d, 0A0h
.text:00000001401C9743 48 8D 0D 8E B4 CC 15                    lea     rcx, unk_155E94BD8
.text:00000001401C974A E9 B1 8B A8 0C                          jmp     sub_14CC52300
.text:00000001401C974A                         sub_1401C9730   endp
.text:00000001401C974A
.text:00000001401C974A                         ; ---------------------------------------------------------------------------
.text:00000001401C974F CC                                      align 10h
.text:00000001401C9750
.text:00000001401C9750                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9750
.text:00000001401C9750
.text:00000001401C9750                         sub_1401C9750   proc near
.text:00000001401C9750 41 B9 01 1B B2 3A                       mov     r9d, 3AB21B01h
.text:00000001401C9756 48 8D 15 E3 89 53 10                    lea     rdx, aUdatatablesyst_12 ; "UDataTableSystemImplDS"
.text:00000001401C975D 41 B8 30 00 00 00                       mov     r8d, 30h ; '0'
.text:00000001401C9763 48 8D 0D 96 B4 CC 15                    lea     rcx, off_155E94C00 ; "egion"
.text:00000001401C976A E9 91 8B A8 0C                          jmp     sub_14CC52300
.text:00000001401C976A                         sub_1401C9750   endp
.text:00000001401C976A
.text:00000001401C976A                         ; ---------------------------------------------------------------------------
.text:00000001401C976F CC                                      align 10h
.text:00000001401C9770
.text:00000001401C9770                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9770
.text:00000001401C9770
.text:00000001401C9770                         sub_1401C9770   proc near
.text:00000001401C9770 41 B9 EC D1 B3 96                       mov     r9d, 96B3D1ECh
.text:00000001401C9776 48 8D 15 7B 8A 53 10                    lea     rdx, aUdatatablesyst_13 ; "UDataTableSystemInterface"
.text:00000001401C977D 41 B8 28 00 00 00                       mov     r8d, 28h ; '('
.text:00000001401C9783 48 8D 0D 9E B4 CC 15                    lea     rcx, unk_155E94C28
.text:00000001401C978A E9 71 8B A8 0C                          jmp     sub_14CC52300
.text:00000001401C978A                         sub_1401C9770   endp
.text:00000001401C978A
.text:00000001401C978A                         ; ---------------------------------------------------------------------------
.text:00000001401C978F CC                                      align 10h
.text:00000001401C9790
.text:00000001401C9790                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9790
.text:00000001401C9790
.text:00000001401C9790                         sub_1401C9790   proc near
.text:00000001401C9790 41 B9 EE 30 B3 3B                       mov     r9d, 3BB330EEh
.text:00000001401C9796 48 8D 15 FB 8D 53 10                    lea     rdx, aUdatatablesyst_14 ; "UDataTableSystemManagerBase"
.text:00000001401C979D 41 B8 30 00 00 00                       mov     r8d, 30h ; '0'
.text:00000001401C97A3 48 8D 0D FE B8 CC 15                    lea     rcx, unk_155E950A8
.text:00000001401C97AA E9 51 8B A8 0C                          jmp     sub_14CC52300
.text:00000001401C97AA                         sub_1401C9790   endp
.text:00000001401C97AA
.text:00000001401C97AA                         ; ---------------------------------------------------------------------------
.text:00000001401C97AF CC                                      align 10h
.text:00000001401C97B0
.text:00000001401C97B0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C97B0
.text:00000001401C97B0
.text:00000001401C97B0                         sub_1401C97B0   proc near
.text:00000001401C97B0 41 B9 B8 BE 2B 23                       mov     r9d, 232BBEB8h
.text:00000001401C97B6 48 8D 15 DB 7E 53 10                    lea     rdx, aUdatatablesyst_15 ; "UDataTableSystemManagerLite"
.text:00000001401C97BD 41 B8 78 02 00 00                       mov     r8d, 278h
.text:00000001401C97C3 48 8D 0D 6E BF CC 15                    lea     rcx, unk_155E95738
.text:00000001401C97CA E9 31 8B A8 0C                          jmp     sub_14CC52300
.text:00000001401C97CA                         sub_1401C97B0   endp
.text:00000001401C97CA
.text:00000001401C97CA                         ; ---------------------------------------------------------------------------
.text:00000001401C97CF CC                                      align 10h
.text:00000001401C97D0
.text:00000001401C97D0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C97D0
.text:00000001401C97D0
.text:00000001401C97D0                         sub_1401C97D0   proc near
.text:00000001401C97D0 41 B9 80 69 3B 74                       mov     r9d, 743B6980h
.text:00000001401C97D6 48 8D 15 73 82 53 10                    lea     rdx, aUdatatablesyst_16 ; "UDataTableSystemManagerLiteInterface"
.text:00000001401C97DD 41 B8 28 00 00 00                       mov     r8d, 28h ; '('
.text:00000001401C97E3 48 8D 0D 9E C1 CC 15                    lea     rcx, unk_155E95988
.text:00000001401C97EA E9 11 8B A8 0C                          jmp     sub_14CC52300
.text:00000001401C97EA                         sub_1401C97D0   endp
.text:00000001401C97EA
.text:00000001401C97EA                         ; ---------------------------------------------------------------------------
.text:00000001401C97EF CC                                      align 10h
.text:00000001401C97F0
.text:00000001401C97F0                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C97F0
.text:00000001401C97F0
.text:00000001401C97F0                         sub_1401C97F0   proc near
.text:00000001401C97F0 41 B9 F4 49 B8 AC                       mov     r9d, 0ACB849F4h
.text:00000001401C97F6 48 8D 15 FB 63 53 10                    lea     rdx, aUdatatablesyst_17 ; "UDataTableSystemPlayerImpl"
.text:00000001401C97FD 41 B8 D8 00 00 00                       mov     r8d, 0D8h
.text:00000001401C9803 48 8D 0D A6 C1 CC 15                    lea     rcx, unk_155E959B0
.text:00000001401C980A E9 F1 8A A8 0C                          jmp     sub_14CC52300
.text:00000001401C980A                         sub_1401C97F0   endp
.text:00000001401C980A
.text:00000001401C980A                         ; ---------------------------------------------------------------------------
.text:00000001401C980F CC                                      align 10h
.text:00000001401C9810
.text:00000001401C9810                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9810
.text:00000001401C9810
.text:00000001401C9810                         sub_1401C9810   proc near
.text:00000001401C9810 41 B9 7E 31 85 C6                       mov     r9d, 0C685317Eh
.text:00000001401C9816 48 8D 15 7B 68 53 10                    lea     rdx, aUdevicesetting ; "UDeviceSetting"
.text:00000001401C981D 41 B8 28 00 00 00                       mov     r8d, 28h ; '('
.text:00000001401C9823 48 8D 0D 4E C5 CC 15                    lea     rcx, unk_155E95D78
.text:00000001401C982A E9 D1 8A A8 0C                          jmp     sub_14CC52300
.text:00000001401C982A                         sub_1401C9810   endp
.text:00000001401C982A
.text:00000001401C982A                         ; ---------------------------------------------------------------------------
.text:00000001401C982F CC                                      align 10h
.text:00000001401C9830
.text:00000001401C9830                         ; =============== S U B R O U T I N E =======================================
.text:00000001401C9830
.text:00000001401C9830
.text:00000001401C9830                         sub_1401C9830   proc near
.text:00000001401C9830 41 B9 CE F2 F2 4C                       mov     r9d, 4CF2F2CEh
.text:00000001401C9836 48 8D 15 9B 6C 53 10                    lea     rdx, aUeditorversion ; "UEditorVersionUtils"
.text:00000001401C983D 41 B8 28 00 00 00                       mov     r8d, 28h ; '('
.text:00000001401C9843 48 8D 0D EE C5 CC 15                    lea     rcx, unk_155E95E38
.text:00000001401C984A E9 B1 8A A8 0C                          jmp     sub_14CC52300
.text:00000001401C984A                         sub_1401C9830   endp

```
