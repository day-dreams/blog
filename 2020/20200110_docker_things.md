# 1. 两个docker相关的小问题

最近碰到两个想深究的问题，工作日没什么时间，趁这会儿夜深人静，抓紧时间看看。
<!-- TOC -->

- [两个docker相关的小问题](#两个docker相关的小问题)
    - [为什么debian基础镜像里不能建立https连接](#为什么debian基础镜像里不能建立https连接)
    - [debian和ubuntu两个镜像有什么不同，怎么选](#debian和ubuntu两个镜像有什么不同怎么选)

<!-- /TOC -->

## 1.1. 为什么debian基础镜像里不能建立https连接

在一个裸的debian镜像里，比如`debian:stable`，无法建立http连接。因为它连基本的证书签名都没有。查看这个镜像里的ssl文件，其实是什么都没有的。没有根证书，自然不能建立ssl握手。
```bash
ls /etc/ssl
ls: cannot access '/etc/ssl': No such file or directory
```

不同语言的标准库，建立https连接可能会使用不同的根证书。可以安装curl，顺带着把这些根证书安装了。

```bash
apt update
apt install curl
ls /etc/ssl
certs  openssl.cnf  private
ls /etc/ssl/certs
 02265526.0
 03179a64.0
 062cdee6.0
 064e0aa9.0
 06dc52d5.0
 080911ac.0
 09789157.0
 0b1b94ef.0
 0bf05006.0
 0c4c9b6c.0
 0f6fa695.0
 106f3e4d.0
 116bf586.0
 128805a3.0
 157753a5.0
 1636090b.0
 18856ac4.0
 1d3472b9.0
 1e08bfd1.0
 1e09d511.0
 244b5494.0
 2ae6433e.0
 2b349938.0
 2c543cd1.0
 2e4eed3c.0
 2e5ac55d.0
 32888f65.0
 349f2832.0
 3513523f.0
 3bde41ac.0
 3e44d2f7.0
 3e45d192.0
 40193066.0
 4042bcee.0
 40547a79.0
 4304c5e5.0
 480720ec.0
 48bec511.0
 4a6481c9.0
 4bfab552.0
 4f316efb.0
 5273a94c.0
 5443e9e3.0
 54657681.0
 57bcb2da.0
 5a4d6896.0
 5ad8a5d6.0
 5c44d531.0
 5cd81ad7.0
 5d3033c5.0
 5f15c80c.0
 607986c7.0
 6410666e.0
 653b494a.0
 6b99d060.0
 6d41d539.0
 6fa5da56.0
 706f604c.0
 749e9e03.0
 75d1b2ed.0
 76cb8f92.0
 76faf6c0.0
 7719f463.0
 773e07ad.0
 7aaf71c0.0
 7d0b38bd.0
 7f3d5d1d.0
 812e17de.0
 8160b96c.0
 8867006a.0
 8cb5ee0f.0
 8d86cdd1.0
 930ac5d2.0
 93bc0acc.0
 988a38cb.0
 9c2e7d30.0
 9c8dfbd4.0
 9d04f354.0
 9f0f5fd6.0
 ACCVRAIZ1.pem
 AC_RAIZ_FNMT-RCM.pem
 Actalis_Authentication_Root_CA.pem
 AddTrust_External_Root.pem
 AffirmTrust_Commercial.pem
 AffirmTrust_Networking.pem
 AffirmTrust_Premium.pem
 AffirmTrust_Premium_ECC.pem
 Amazon_Root_CA_1.pem
 Amazon_Root_CA_2.pem
 Amazon_Root_CA_3.pem
 Amazon_Root_CA_4.pem
 Atos_TrustedRoot_2011.pem
 Autoridad_de_Certificacion_Firmaprofesional_CIF_A62634068.pem
 Baltimore_CyberTrust_Root.pem
 Buypass_Class_2_Root_CA.pem
 Buypass_Class_3_Root_CA.pem
 CA_Disig_Root_R2.pem
 CFCA_EV_ROOT.pem
 COMODO_Certification_Authority.pem
 COMODO_ECC_Certification_Authority.pem
 COMODO_RSA_Certification_Authority.pem
 Certigna.pem
 Certinomis_-_Root_CA.pem
 Certplus_Class_2_Primary_CA.pem
 Certum_Trusted_Network_CA.pem
 Certum_Trusted_Network_CA_2.pem
 Chambers_of_Commerce_Root_-_2008.pem
 Comodo_AAA_Services_root.pem
 Cybertrust_Global_Root.pem
 D-TRUST_Root_Class_3_CA_2_2009.pem
 D-TRUST_Root_Class_3_CA_2_EV_2009.pem
 DST_Root_CA_X3.pem
 Deutsche_Telekom_Root_CA_2.pem
 DigiCert_Assured_ID_Root_CA.pem
 DigiCert_Assured_ID_Root_G2.pem
 DigiCert_Assured_ID_Root_G3.pem
 DigiCert_Global_Root_CA.pem
 DigiCert_Global_Root_G2.pem
 DigiCert_Global_Root_G3.pem
 DigiCert_High_Assurance_EV_Root_CA.pem
 DigiCert_Trusted_Root_G4.pem
 E-Tugra_Certification_Authority.pem
 EC-ACC.pem
 EE_Certification_Centre_Root_CA.pem
 Entrust.net_Premium_2048_Secure_Server_CA.pem
 Entrust_Root_Certification_Authority.pem
 Entrust_Root_Certification_Authority_-_EC1.pem
 Entrust_Root_Certification_Authority_-_G2.pem
 GDCA_TrustAUTH_R5_ROOT.pem
 GeoTrust_Global_CA.pem
 GeoTrust_Primary_Certification_Authority.pem
 GeoTrust_Primary_Certification_Authority_-_G2.pem
 GeoTrust_Primary_Certification_Authority_-_G3.pem
 GeoTrust_Universal_CA.pem
 GeoTrust_Universal_CA_2.pem
 GlobalSign_ECC_Root_CA_-_R4.pem
 GlobalSign_ECC_Root_CA_-_R5.pem
 GlobalSign_Root_CA.pem
 GlobalSign_Root_CA_-_R2.pem
 GlobalSign_Root_CA_-_R3.pem
 GlobalSign_Root_CA_-_R6.pem
 Global_Chambersign_Root_-_2008.pem
 Go_Daddy_Class_2_CA.pem
 Go_Daddy_Root_Certificate_Authority_-_G2.pem
 Hellenic_Academic_and_Research_Institutions_ECC_RootCA_2015.pem
 Hellenic_Academic_and_Research_Institutions_RootCA_2011.pem
 Hellenic_Academic_and_Research_Institutions_RootCA_2015.pem
 Hongkong_Post_Root_CA_1.pem
 ISRG_Root_X1.pem
 IdenTrust_Commercial_Root_CA_1.pem
 IdenTrust_Public_Sector_Root_CA_1.pem
 Izenpe.com.pem
 LuxTrust_Global_Root_2.pem
 Microsec_e-Szigno_Root_CA_2009.pem
'NetLock_Arany_=Class_Gold=_F'$'\305\221''tan'$'\303\272''s'$'\303\255''tv'$'\303\241''ny.pem'
 Network_Solutions_Certificate_Authority.pem
 OISTE_WISeKey_Global_Root_GA_CA.pem
 OISTE_WISeKey_Global_Root_GB_CA.pem
 OISTE_WISeKey_Global_Root_GC_CA.pem
 QuoVadis_Root_CA.pem
 QuoVadis_Root_CA_1_G3.pem
 QuoVadis_Root_CA_2.pem
 QuoVadis_Root_CA_2_G3.pem
 QuoVadis_Root_CA_3.pem
 QuoVadis_Root_CA_3_G3.pem
 SSL.com_EV_Root_Certification_Authority_ECC.pem
 SSL.com_EV_Root_Certification_Authority_RSA_R2.pem
 SSL.com_Root_Certification_Authority_ECC.pem
 SSL.com_Root_Certification_Authority_RSA.pem
 SZAFIR_ROOT_CA2.pem
 SecureSign_RootCA11.pem
 SecureTrust_CA.pem
 Secure_Global_CA.pem
 Security_Communication_RootCA2.pem
 Security_Communication_Root_CA.pem
 Sonera_Class_2_Root_CA.pem
 Staat_der_Nederlanden_EV_Root_CA.pem
 Staat_der_Nederlanden_Root_CA_-_G2.pem
 Staat_der_Nederlanden_Root_CA_-_G3.pem
 Starfield_Class_2_CA.pem
 Starfield_Root_Certificate_Authority_-_G2.pem
 Starfield_Services_Root_Certificate_Authority_-_G2.pem
 SwissSign_Gold_CA_-_G2.pem
 SwissSign_Silver_CA_-_G2.pem
 T-TeleSec_GlobalRoot_Class_2.pem
 T-TeleSec_GlobalRoot_Class_3.pem
 TUBITAK_Kamu_SM_SSL_Kok_Sertifikasi_-_Surum_1.pem
 TWCA_Global_Root_CA.pem
 TWCA_Root_Certification_Authority.pem
 Taiwan_GRCA.pem
 TeliaSonera_Root_CA_v1.pem
 TrustCor_ECA-1.pem
 TrustCor_RootCert_CA-1.pem
 TrustCor_RootCert_CA-2.pem
 Trustis_FPS_Root_CA.pem
 USERTrust_ECC_Certification_Authority.pem
 USERTrust_RSA_Certification_Authority.pem
 VeriSign_Class_3_Public_Primary_Certification_Authority_-_G4.pem
 VeriSign_Class_3_Public_Primary_Certification_Authority_-_G5.pem
 VeriSign_Universal_Root_Certification_Authority.pem
 Verisign_Class_3_Public_Primary_Certification_Authority_-_G3.pem
 XRamp_Global_CA_Root.pem
 a94d09e5.0
 ad088e1d.0
 aee5f10d.0
 b0e59380.0
 b1159c4c.0
 b1b8a7f3.0
 b204d74a.0
 b66938e9.0
 b727005e.0
 b7a5b843.0
 ba89ed3b.0
 c01cdfa2.0
 c089bbbd.0
 c0ff1f52.0
 c28a8a30.0
 c47d9980.0
 ca-certificates.crt
 ca6e4ad9.0
 cbf06781.0
 cc450945.0
 cd58d51e.0
 cd8c0d63.0
 ce5e74ef.0
 certSIGN_ROOT_CA.pem
 d4dae3dd.0
 d6325660.0
 d7e8dc79.0
 d853d49e.0
 dc4d6a89.0
 dd8e9d41.0
 de6d66f3.0
 def36a68.0
 e113c810.0
 e18bfb83.0
 e2799e36.0
 e36a6752.0
 e73d606e.0
 e8de2f56.0
 ePKI_Root_Certification_Authority.pem
 ee64a828.0
 eed8c118.0
 ef954a4e.0
 f060240e.0
 f081611a.0
 f0c70a8d.0
 f30dd6ad.0
 f3377b1b.0
 f387163d.0
 f39fc864.0
 fc5a8f99.0
 fe8a2cd8.0
 ff34af3f.0
 thawte_Primary_Root_CA.pem
 thawte_Primary_Root_CA_-_G2.pem
```

## 1.2. debian和ubuntu两个镜像有什么不同，怎么选

别想这个问题了，它属于linux发行版的知识范畴。我们写应用的，不用钻的太深。

这里只记录几条tips：
* `alpine`镜像，很简洁的镜像，简洁到可能缺少程序运行必要的基础库。很可能你的程序编译好了，放到这个镜像里跑不动。所以有人推荐你用这个镜像的话，必有问题
* `debian`镜像，mysql和redis官方镜像都是基于它的，放心使用。
* `ubuntu`镜像，学生时代老是折腾ubuntu，就觉得它比debian多了很多东西，不够简洁。**但是**，mongo官方镜像，就用的ubuntu当基础镜像。而且它和debian镜像大小差不了多少，所以放心使用吧。至于它和ubuntu的差别，真的不用深究了。
* 很多基础镜像都没有安装ssl根证书，可以通过安装`curl`，顺带着把证书都给安装了。