<p align="center">
    <img src="./Additionals/Empa-Workshops-Template-Banner2.png" alt="Accelerator Workshops" 
    style="display: block; margin: 0 auto"/>
</p>

# 2) ST Platformlarında Uçta Yapay Zeka Çözümleri Geliştirme

## Çalışma Ortamı Kurulumu
### Bulutta Çalışma Ortamı (Sadece Geliştirme)
Aktivite için oluşturulan bulut çalışma ortamı _geliştirme_ ortamı için ortam kurulumu gerektirmez. Geliştirilen çözümün testi için gerekli test ortamı seri port bağlantısı için yerel çalışma ortamı gerektirir, bulutta çalışma ortamında **kullanılamaz**. Aşağıdaki linke tıklayarak erişebileceğiniz Colab Notebook örneği ile aktivitenin _geliştirme_ adımlarına uygulamalı katılım sağlayabilirsiniz.

**1- Colab Notebook Örneği**  
[Accelerators Workshop - Uçta Yapay Zeka - Aktivite-2 Bulutta Geliştirme Çalışma Ortamı](https://colab.research.google.com/drive/1rSpHM_JGy5NAFCWkZ7TPcCt1fxc8kywF?usp=sharing)

### Yerel Çalışma Ortamı (Sadece Test)

## 1- STM32CubeIDE Kurulumu

STM32CubeIDE, STM32 mikrodenetleyicileri ve mikroişlemcileri için gerekli çevresel yapılandırmaların yapılmasını, kod oluşturma ve derlemeyi, kodun hata ayıklama işlemlerini ve cihaza yüklenmesini sağlayan Eclipse tabanlı bir C/C++ geliştirme platformudur.

Öncelikle [buradan](https://www.st.com/en/development-tools/stm32cubeide.html#get-software) STM32CubeIDE 1.17.0 (en son sürüm) versiyonunu indiriniz.
Aşağıdaki seçenekler arasından işletim sisteminize uygun olanı indirip kurulumunu gerçekleştirin. İndirme işlemini gerçekleştirmek için **lisans sözleşmesini onaylamanız** ve **MyST hesabı oluşturup giriş yapmanız** gerekmektedir.
<br />
<div align="center">
  <img width="100%" height="100%" src="Additionals/kurulum1.png">
</div>
<br />

NOT 1: Kurulum yapacağınız hedef klasör yolunda Türkçe karakter ve boşluk olmadığına dikkat ediniz.

NOT 2: Kurulum sırasında ST-LINK ve SEGGER J-Link sürücülerinin yüklenip yüklenmeyeceği sorulacaktır. Bu sürücüleri seçip yükleme işlemlerine devam ediniz.

## 2- STM32CubeProgrammer Kurulumu

STM32CubeProgrammer, STM32 mikrodenetleyicileri programlama ve konfigürasyonlarını ayarlamak için tasarlanmış grafiksel bir araçtır.
<br />Öncelikle [buradan](https://www.st.com/en/development-tools/stm32cubeprog.html#get-software) **STM32CubePrg 2.18.0** (son sürüm) versiyonunu indiriniz. Aşağıdaki seçenekler arasından işletim sisteminize uygun olanı indirip kurulumunu gerçekleştirin. İndirme işlemini gerçekleştirmek için **lisans sözleşmesini onaylamanız** ve **myST hesabınıza giriş yapmış olmanız** gerekmektedir.

<br />
<div align="center">
  <img width="100%" height="100%" src="Additionals/cubeprogrammer.png">
</div>
<br />

## 3- Tera Term Kurulumu
Tera Term, kullanıcıların seri port iletişimi sağlayarak uzak bilgisayarlara erişmelerine ve UART gibi çeşitli iletişim protokolleri aracılığıyla cihazlarla iletişim kurmalarına olanak tanır. Bilgisayar ile geliştirme kartı arasında UART üzerinden iletişim sağlamak için bu program kullanılacaktır.
Github [sayfasından](https://github.com/TeraTermProject/teraterm/releases) Assets altından dosyasını indiriniz ve kurulumu gerçekleştiriniz.<br />
<div align="center">
  <img width="100%" height="100%" src="Additionals/teraterm.png">
</div>
<br />

## 4- SW Paketlerinin Kurulumu
Öncelikle paketlerin kurulumu işlemi için MySt hesabının girilmesi gerekmektedir. Bunun için **Help** sekmesinin altındaki **STM32Cube updates** seçeneğinin yanından **Connection to myST** seçeneğinin seçilmesi gerekmektedir. 
<div align="center">
  <img width="100%" height="100%" src="Additionals/myst_login.png">
</div>

<br />

Ardından açılan ekranda **Enter myST account information seçeneğine** tıklayın.
<div align="center">
  <img width="100%" height="100%" src="Additionals/myst_connection.png">
</div>
<br />

Ve hesabınıza giriş yapın.
<div align="center">
  <img width="100%" height="100%" src="Additionals/login.png">
</div>
<br />

# 4-A) U5 Paketinin Kurulumu
Uygulamalarda STM32U5 ailesinden bir mikrodenetleyici kullanılacağı için U5 paketlerinin indirilmesi gereklidir.<br />
Öncelikle STM32CubeIDE programını başlatınız.

Açılan STM32CubeIDE uygulaması içerisinde **Help** sekmesi altında **Configuration Tools** seçeneğinin yanındaki **Manage Embedded Software Packages** seçeneğine tıklayınız.
<div align="center">
  <img width="100%" height="100%" src="Additionals/manage_embedded_software_packages.png">
</div>

<br />

Ardından **STM32Cube MCU Packages** sekmesi altında **STM32U5->STM32Cube MCU Package for STM32U5 Series** yanındaki **1.7.0** (en son sürüm) sürümününün kutucuğunu işaretleyiniz ve install'a tıklayarak yükleme işlemini gerçekleştiriniz. <br />
<div align="center">
  <img width="100%" height="100%" src="Additionals/u5_install.png">
</div>
<br />

# 4-B) X-CUBE-ISPU Yazılım Paketi Kurulumu 
X-CUBE-ISPU yazılım paketinin yüklenmesi için STM32CubeIDE uygulaması içerisinde **Help** sekmesi altında **Configuration Tools** seçeneğinin yanındaki **Manage Embedded Software Packages** seçeneğine tıklayınız.
<br />
<div align="center">
  <img width="100%" height="100%" src="Additionals/manage_embedded_software_packages.png">
</div>
<br />

Açılan ekranda yukarıdaki sekmelerden STMicroelectronics seçilir. Ardından paketlerden X-CUBE-ISPU paketi seçilir. Ve **2.1.0 versiyonunun** (en son sürümü indiriniz) yanındaki kutucuk işaretlenir. Ve install'a tıklanarak indirme işlemi başlatılır. Yükleme işlemi sırasında kullanıcı girişinin yapılmış olması ve lisans sözleşmesini kabul edilmesi gerekmektedir. Paketin yüklenme işlemi tamamlanmıştır. 

<br />
<div align="center">
  <img width="100%" height="100%" src="Additionals/ispu_install.png">
</div>
<br />

# 4-C) X-CUBE-AI Yazılım Paketi Kurulumu 
X-CUBE-AI yazılım paketinin yüklenmesi için STM32CubeIDE uygulaması içerisinde **Help** sekmesi altında **Configuration Tools** seçeneğinin yanındaki **Manage Embedded Software Packages** seçeneğine tıklayınız.
<br />
<div align="center">
  <img width="100%" height="100%" src="Additionals/manage_embedded_software_packages.png">
</div>
<br />

Açılan ekranda yukarıdaki sekmelerden STMicroelectronics seçilir. Ardından paketlerden X-CUBE-AI paketi seçilir. Ve **10.0.0 versiyonunun** (son sürüm) yanındaki kutucuk işaretlenir. Ve install'a tıklanarak indirme işlemi başlatılır. Yükleme işlemi sırasında kullanıcı girişinin yapılmış olması ve lisans anlaşmasının kabul edilmesi gerekmektedir. Paketin yüklenme işlemi tamamlanmıştır.

<br />
<div align="center">
  <img width="100%" height="100%" src="Additionals/cubeai_package.png">
</div>
<br />

Gerekli kurulumlar tamamlanmıştır.
