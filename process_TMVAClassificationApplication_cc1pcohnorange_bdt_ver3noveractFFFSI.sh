#!/bin/bash
#root -b -q TMVAClassificationApplication_cc1pcohnorange_bdt_ver3noveractFFFSI.C\(\"BDT\",\"/home/cvson/cc1picoh/FIT/datafsicornorange/datamc_merged_ccqe_addpidFFnew_addnue.root\"\)
#root -b -q TMVAClassificationApplication_cc1pcohnorange_bdt_ver3noveractFFFSI.C\(\"BDT\",\"/home/cvson/cc1picoh/FIT/datafsicornorange/datamc_genie_merged_ccqe_addpidFFnew_addnue.root\"\)

###For PE
#for i in {0..8}
#do
#echo "/home/cvson/cc1picoh/FIT/systfsi/frKikawa/MC/pm/pe/merged_ccqe_${i}.root"
#root -b -q TMVAClassificationApplication_cc1pcohnorange_bdt_ver3noveractFFFSI.C\(\"BDT\",\"/home/cvson/cc1picoh/FIT/systfsi/frKikawa/MC/pm/pe/merged_ccqe_${i}.root\"\);
#done

###For hit efficiency
#for i in {2..6}
#do
#echo "/home/cvson/cc1picoh/FIT/systfsi/frKikawa/MC/pm/hiteff/merged_ccqe_${i}.root"
#root -b -q TMVAClassificationApplication_cc1pcohnorange_bdt_ver3noveractFFFSI.C\(\"BDT\",\"/home/cvson/cc1picoh/FIT/systfsi/frKikawa/MC/pm/hiteff/merged_ccqe_${i}.root\"\);
#done

###For MC reconstruction
#for FILE in `cat /home/cvson/cc1picoh/FIT/systfsi/frKikawa/MC/pm/sys/list_file.txt`;
#do
#echo "/home/cvson/cc1picoh/FIT/systfsi/frKikawa/MC/pm/sys/${FILE}"
#root -b -q TMVAClassificationApplication_cc1pcohnorange_bdt_ver3noveractFFFSI.C\(\"BDT\",\"/home/cvson/cc1picoh/FIT/systfsi/frKikawa/MC/pm/sys/${FILE}\"\);
#done

###For data reconstruction

#for FILE in `cat /home/cvson/cc1picoh/FIT/systfsi/frKikawa/data/sys/list_file.txt`;
#do
#echo "/home/cvson/cc1picoh/FIT/systfsi/frKikawa/data/sys/${FILE}"
#root -b -q TMVAClassificationApplication_cc1pcohnorange_bdt_ver3noveractFFFSI.C\(\"BDT\",\"/home/cvson/cc1picoh/FIT/systfsi/frKikawa/data/sys/${FILE}\"\);
#done

###For noise

#for FILE in `cat /home/cvson/cc1picoh/FIT/systfsi/frKikawa/MC/pm/noise/list_file.txt`;
#do
#echo "/home/cvson/cc1picoh/FIT/systfsi/frKikawa/MC/pm/noise/${FILE}"
#root -b -q TMVAClassificationApplication_cc1pcohnorange_bdt_ver3noveractFFFSI.C\(\"BDT\",\"/home/cvson/cc1picoh/FIT/systfsi/frKikawa/MC/pm/noise/${FILE}\"\);
#done
root -b -q TMVAClassificationApplication_cc1pcohnorange_bdt_ver3noveractFFFSI.C\(\"BDT\",\"/home/cvson/cc1picoh/FIT/datafsicornorange/merged_ccqe_forResponseFunction.root\"\)
