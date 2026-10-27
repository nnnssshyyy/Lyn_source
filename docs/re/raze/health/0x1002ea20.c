int __usercall sub_1002EA20@<eax>(
        char **a1@<ecx>,
        __int128 _XMM3@<xmm3>,
        __int128 _XMM4@<xmm4>,
        __int128 _XMM5@<xmm5>,
        __int128 _XMM6@<xmm6>)
{
  char **v6; // edi
  char *v7; // ebx
  char *v8; // ecx
  unsigned int v9; // eax
  _DWORD *v10; // ecx
  unsigned int v11; // edi
  char *v12; // ebx
  char *v13; // ecx
  unsigned int v14; // eax
  _DWORD *v15; // ecx
  unsigned int v16; // edi
  char *v17; // ebx
  char *v18; // ecx
  unsigned int v19; // eax
  _DWORD *v20; // ecx
  unsigned int v21; // edi
  char *v22; // ebx
  char *v23; // ecx
  unsigned int v24; // edi
  _DWORD *v25; // ecx
  unsigned int v26; // eax
  char *v27; // ebx
  char *v28; // ecx
  unsigned int v29; // eax
  _DWORD *v30; // ecx
  unsigned int v31; // edi
  char *v32; // eax
  int result; // eax
  int (__thiscall *v34)(int); // eax
  char v36; // al
  char ***v37; // eax
  char **v38; // edi
  wchar_t *v39; // ebx
  int (__thiscall *v40)(char *); // eax
  void (*v45)(void); // eax
  int (__thiscall *v46)(int, int); // eax
  int v47; // eax
  char *v48; // ebx
  int v49; // edi
  int v50; // eax
  int v51; // edi
  int v52; // eax
  char *v62; // ecx
  int v63; // eax
  char v89; // cf
  char v90; // zf
  char *v103; // ecx
  int v104; // eax
  int v127; // edi
  int v172; // edi
  int v206; // ebx
  int v208; // edi
  int v209; // eax
  int v223; // edi
  int v224; // eax
  wchar_t *v232; // edi
  char *v233; // ecx
  int v250; // edi
  int v251; // eax
  int v265; // edi
  int v266; // eax
  wchar_t *v274; // edi
  char *v275; // ecx
  int (__thiscall *v276)(char *); // eax
  unsigned __int8 (*v285)(void); // eax
  int v286; // eax
  int (__thiscall *v287)(char *); // eax
  int (__thiscall *v299)(char *); // eax
  int v300; // edi
  char v301; // dl
  int (__thiscall *v309)(char *); // eax
  int v310; // edi
  char v311; // dl
  char v321; // al
  int v329; // edi
  int (__thiscall *v341)(char *); // eax
  int (__thiscall *v350)(char *); // eax
  int (__thiscall *v362)(char *); // eax
  int v363; // edi
  char v364; // dl
  int (__thiscall *v372)(char *); // eax
  int v373; // edi
  char v374; // dl
  wchar_t *v391; // ebx
  char *v405; // ebx
  int v430; // edx
  int v435; // edi
  int v447; // eax
  int v469; // edx
  int v474; // edi
  char *v476; // ebx
  void (__thiscall *v493)(int, int); // eax
  char *v494; // edx
  int v499; // eax
  int v500; // eax
  int v501; // eax
  int v502; // ebx
  int v503; // edi
  int v504; // eax
  int v505; // edi
  int v510; // eax
  int v511; // eax
  int v512; // edi
  int v513; // edi
  _WORD *v514; // eax
  int v609; // edi
  _WORD *v610; // eax
  int v670; // eax
  int v671; // ecx
  int v672; // eax
  unsigned int v686; // edx
  int v688; // eax
  unsigned int v707; // edi
  char v716; // di
  int v727; // eax
  wchar_t *v732; // edi
  int v739; // eax
  int v740; // ebx
  int v741; // eax
  int v750; // eax
  int v751; // ebx
  int v752; // eax
  wchar_t *v773; // edi
  int v794; // eax
  int v805; // ebx
  int v806; // edi
  int v822; // eax
  char v861; // al
  void (__thiscall *v992)(int, int); // eax
  wchar_t **v993; // eax
  wchar_t *v994; // edi
  int v995; // ebx
  char ***v1000; // eax
  char **v1001; // edi
  int v1010; // ebx
  int v1027; // eax
  int v1040; // edx
  int (*v1103)(void); // eax
  char *v1104; // ebx
  char *v1119; // ecx
  char *v1123; // edi
  long double v1126; // st7
  float *v1127; // eax
  long double v1129; // st7
  float *v1130; // eax
  unsigned int v1145; // eax
  int v1168; // eax
  int v1176; // edi
  int v1197; // eax
  char *v1203; // edi
  char ***v1207; // eax
  char **v1208; // ebx
  char *v1209; // edx
  int v1210; // eax
  int v1211; // eax
  int v1212; // eax
  _DWORD *v1213; // ebx
  int v1214; // edi
  int v1215; // eax
  int v1232; // eax
  char *v1233; // ecx
  int v1250; // eax
  char *v1251; // edx
  int v1252; // ecx
  const char *v1253; // ebx
  __int128 *v1254; // eax
  int v1271; // ebx
  int v1272; // eax
  int v1283; // eax
  int v1294; // eax
  int v1295; // eax
  int v1296; // eax
  int v1297; // eax
  int v1298; // eax
  int v1299; // edi
  int v1300; // eax
  int v1309; // eax
  char v1429; // al
  int v1430; // eax
  int v1441; // eax
  int v1442; // eax
  int v1443; // eax
  int v1444; // eax
  int v1445; // eax
  int v1446; // edi
  int v1447; // eax
  int v1456; // eax
  char v1518; // al
  int v1519; // eax
  int v1520; // ebx
  int v1521; // eax
  int (__stdcall *v1659)(_DWORD); // eax
  char *v1661; // ecx
  int (*v1662)(void); // eax
  void (__thiscall *v1667)(char *); // eax
  char *v1680; // edi
  char *j; // eax
  unsigned int k; // edi
  int v1722; // [esp-Ch] [ebp-464h]
  _BYTE v1723[20]; // [esp-8h] [ebp-460h] BYREF
  char v1724[20]; // [esp+Ch] [ebp-44Ch] BYREF
  char *v1725; // [esp+20h] [ebp-438h]
  char **i; // [esp+24h] [ebp-434h]
  unsigned int v1727; // [esp+28h] [ebp-430h]
  wchar_t *v1728; // [esp+2Ch] [ebp-42Ch]
  int v1729; // [esp+30h] [ebp-428h]
  char *v1730; // [esp+34h] [ebp-424h]
  int v1731; // [esp+3Ch] [ebp-41Ch]
  _DWORD v1732[2]; // [esp+40h] [ebp-418h] BYREF
  char *v1733; // [esp+48h] [ebp-410h]
  wchar_t *v1734; // [esp+4Ch] [ebp-40Ch]
  int v1735; // [esp+60h] [ebp-3F8h]
  int v1736; // [esp+64h] [ebp-3F4h] BYREF
  int v1737; // [esp+68h] [ebp-3F0h] BYREF
  wchar_t *v1738; // [esp+6Ch] [ebp-3ECh]
  int v1739; // [esp+88h] [ebp-3D0h] BYREF
  int v1740; // [esp+8Ch] [ebp-3CCh]
  float v1741[6]; // [esp+9Ch] [ebp-3BCh] BYREF
  _DWORD v1742[5]; // [esp+B4h] [ebp-3A4h] BYREF
  char **v1743; // [esp+C8h] [ebp-390h]
  _DWORD v1744[5]; // [esp+CCh] [ebp-38Ch] BYREF
  unsigned int v1745; // [esp+E0h] [ebp-378h]
  char **v1746; // [esp+E4h] [ebp-374h]
  int v1747; // [esp+E8h] [ebp-370h] BYREF
  char *v1748; // [esp+ECh] [ebp-36Ch]
  unsigned int v1749; // [esp+F0h] [ebp-368h]
  wchar_t *v1750; // [esp+F4h] [ebp-364h]
  int v1751; // [esp+F8h] [ebp-360h]
  char *v1752; // [esp+FCh] [ebp-35Ch]
  wchar_t *v1753; // [esp+100h] [ebp-358h]
  wchar_t *v1754; // [esp+104h] [ebp-354h]
  int v1755; // [esp+108h] [ebp-350h] BYREF
  char *v1756; // [esp+10Ch] [ebp-34Ch]
  wchar_t *v1757; // [esp+110h] [ebp-348h]
  char *v1758; // [esp+114h] [ebp-344h]
  wchar_t *v1759; // [esp+118h] [ebp-340h]
  wchar_t *v1760; // [esp+11Ch] [ebp-33Ch]
  int v1761; // [esp+120h] [ebp-338h]
  int v1762; // [esp+124h] [ebp-334h]
  int v1763; // [esp+128h] [ebp-330h]
  char v1764; // [esp+12Ch] [ebp-32Ch]
  char v1765; // [esp+12Dh] [ebp-32Bh]
  char v1766; // [esp+12Eh] [ebp-32Ah]
  char v1767; // [esp+12Fh] [ebp-329h]
  int v1768; // [esp+130h] [ebp-328h]
  char v1769; // [esp+134h] [ebp-324h]
  char v1770; // [esp+135h] [ebp-323h]
  char v1771; // [esp+136h] [ebp-322h]
  char v1772; // [esp+137h] [ebp-321h]
  char v1773; // [esp+138h] [ebp-320h]
  char v1774; // [esp+139h] [ebp-31Fh]
  char v1775; // [esp+13Ah] [ebp-31Eh]
  char v1776; // [esp+13Bh] [ebp-31Dh]
  char v1777; // [esp+13Ch] [ebp-31Ch]
  char v1778; // [esp+13Dh] [ebp-31Bh]
  char v1779; // [esp+13Eh] [ebp-31Ah]
  char v1780; // [esp+13Fh] [ebp-319h]
  char v1781; // [esp+140h] [ebp-318h]
  char v1782; // [esp+141h] [ebp-317h]
  char v1783; // [esp+142h] [ebp-316h]
  char v1784; // [esp+143h] [ebp-315h]
  char v1785; // [esp+144h] [ebp-314h]
  char v1786; // [esp+145h] [ebp-313h]
  char v1787; // [esp+146h] [ebp-312h]
  char v1788; // [esp+147h] [ebp-311h]
  char v1789; // [esp+148h] [ebp-310h]
  char v1790; // [esp+149h] [ebp-30Fh]
  char v1791; // [esp+14Ah] [ebp-30Eh]
  char v1792; // [esp+14Bh] [ebp-30Dh]
  int v1793; // [esp+14Ch] [ebp-30Ch]
  char v1794; // [esp+150h] [ebp-308h]
  char v1795; // [esp+151h] [ebp-307h]
  char v1796; // [esp+152h] [ebp-306h]
  char v1797; // [esp+153h] [ebp-305h]
  char v1798; // [esp+154h] [ebp-304h]
  char v1799; // [esp+155h] [ebp-303h]
  char v1800; // [esp+156h] [ebp-302h]
  char v1801; // [esp+157h] [ebp-301h]
  char v1802; // [esp+158h] [ebp-300h]
  char v1803; // [esp+159h] [ebp-2FFh]
  char v1804; // [esp+15Ah] [ebp-2FEh]
  char v1805; // [esp+15Bh] [ebp-2FDh]
  char v1806; // [esp+15Ch] [ebp-2FCh]
  char v1807; // [esp+15Dh] [ebp-2FBh]
  char v1808; // [esp+15Eh] [ebp-2FAh]
  char v1809; // [esp+15Fh] [ebp-2F9h]
  char v1810; // [esp+160h] [ebp-2F8h]
  char v1811; // [esp+161h] [ebp-2F7h]
  char v1812; // [esp+162h] [ebp-2F6h]
  char v1813; // [esp+163h] [ebp-2F5h]
  int v1814; // [esp+164h] [ebp-2F4h]
  char v1815; // [esp+168h] [ebp-2F0h]
  char v1816; // [esp+169h] [ebp-2EFh]
  char v1817; // [esp+16Ah] [ebp-2EEh]
  char v1818; // [esp+16Bh] [ebp-2EDh]
  char v1819; // [esp+16Ch] [ebp-2ECh]
  char v1820; // [esp+16Dh] [ebp-2EBh]
  char v1821; // [esp+16Eh] [ebp-2EAh]
  char v1822; // [esp+16Fh] [ebp-2E9h]
  char v1823; // [esp+170h] [ebp-2E8h]
  char v1824; // [esp+171h] [ebp-2E7h]
  char v1825; // [esp+172h] [ebp-2E6h]
  char v1826; // [esp+173h] [ebp-2E5h]
  char v1827; // [esp+174h] [ebp-2E4h]
  char v1828; // [esp+175h] [ebp-2E3h]
  char v1829; // [esp+176h] [ebp-2E2h]
  char v1830; // [esp+177h] [ebp-2E1h]
  char v1831; // [esp+178h] [ebp-2E0h]
  char v1832; // [esp+179h] [ebp-2DFh]
  char v1833; // [esp+17Ah] [ebp-2DEh]
  char v1834; // [esp+17Bh] [ebp-2DDh]
  char v1835; // [esp+17Ch] [ebp-2DCh]
  char v1836; // [esp+17Dh] [ebp-2DBh]
  char v1837; // [esp+17Eh] [ebp-2DAh]
  char v1838; // [esp+17Fh] [ebp-2D9h]
  int v1839; // [esp+180h] [ebp-2D8h]
  char v1840; // [esp+184h] [ebp-2D4h]
  char v1841; // [esp+185h] [ebp-2D3h]
  char v1842; // [esp+186h] [ebp-2D2h]
  char v1843; // [esp+187h] [ebp-2D1h]
  int v1844; // [esp+188h] [ebp-2D0h]
  char v1845; // [esp+18Ch] [ebp-2CCh]
  char v1846; // [esp+18Dh] [ebp-2CBh]
  char v1847; // [esp+18Eh] [ebp-2CAh]
  char v1848; // [esp+18Fh] [ebp-2C9h]
  int v1849; // [esp+190h] [ebp-2C8h]
  char v1850; // [esp+194h] [ebp-2C4h]
  char v1851; // [esp+195h] [ebp-2C3h]
  char v1852; // [esp+196h] [ebp-2C2h]
  char v1853; // [esp+197h] [ebp-2C1h]
  char v1854; // [esp+198h] [ebp-2C0h]
  char v1855; // [esp+199h] [ebp-2BFh]
  char v1856; // [esp+19Ah] [ebp-2BEh]
  char v1857; // [esp+19Bh] [ebp-2BDh]
  char v1858; // [esp+19Ch] [ebp-2BCh]
  char v1859; // [esp+19Dh] [ebp-2BBh]
  char v1860; // [esp+19Eh] [ebp-2BAh]
  char v1861; // [esp+19Fh] [ebp-2B9h]
  char v1862; // [esp+1A0h] [ebp-2B8h]
  char v1863; // [esp+1A1h] [ebp-2B7h]
  char v1864; // [esp+1A2h] [ebp-2B6h]
  char v1865; // [esp+1A3h] [ebp-2B5h]
  int v1866; // [esp+1A4h] [ebp-2B4h]
  char v1867; // [esp+1A8h] [ebp-2B0h]
  char v1868; // [esp+1A9h] [ebp-2AFh]
  char v1869; // [esp+1AAh] [ebp-2AEh]
  char v1870; // [esp+1ABh] [ebp-2ADh]
  int v1871; // [esp+1ACh] [ebp-2ACh]
  char v1872; // [esp+1B0h] [ebp-2A8h]
  char v1873; // [esp+1B1h] [ebp-2A7h]
  char v1874; // [esp+1B2h] [ebp-2A6h]
  char v1875; // [esp+1B3h] [ebp-2A5h]
  char v1876; // [esp+1B4h] [ebp-2A4h]
  char v1877; // [esp+1B5h] [ebp-2A3h]
  char v1878; // [esp+1B6h] [ebp-2A2h]
  char v1879; // [esp+1B7h] [ebp-2A1h]
  char v1880; // [esp+1B8h] [ebp-2A0h]
  char v1881; // [esp+1B9h] [ebp-29Fh]
  char v1882; // [esp+1BAh] [ebp-29Eh]
  char v1883; // [esp+1BBh] [ebp-29Dh]
  char v1884; // [esp+1BCh] [ebp-29Ch]
  char v1885; // [esp+1BDh] [ebp-29Bh]
  char v1886; // [esp+1BEh] [ebp-29Ah]
  char v1887; // [esp+1BFh] [ebp-299h]
  char v1888; // [esp+1C0h] [ebp-298h]
  char v1889; // [esp+1C1h] [ebp-297h]
  char v1890; // [esp+1C2h] [ebp-296h]
  char v1891; // [esp+1C3h] [ebp-295h]
  char v1892; // [esp+1C4h] [ebp-294h]
  char v1893; // [esp+1C5h] [ebp-293h]
  char v1894; // [esp+1C6h] [ebp-292h]
  char v1895; // [esp+1C7h] [ebp-291h]
  int v1896; // [esp+1C8h] [ebp-290h] BYREF
  float *v1897; // [esp+1CCh] [ebp-28Ch] BYREF
  char *v1898; // [esp+1D0h] [ebp-288h]
  _DWORD v1899[37]; // [esp+1D4h] [ebp-284h] BYREF
  int v1900; // [esp+268h] [ebp-1F0h]
  wchar_t *v1901; // [esp+26Ch] [ebp-1ECh]
  float v1902; // [esp+294h] [ebp-1C4h]
  int v1903; // [esp+298h] [ebp-1C0h]
  int v1904; // [esp+29Ch] [ebp-1BCh]
  char **v1905; // [esp+2A0h] [ebp-1B8h]
  char **v1906; // [esp+2A4h] [ebp-1B4h]
  char **v1907; // [esp+2A8h] [ebp-1B0h]
  char **v1908; // [esp+2ACh] [ebp-1ACh]
  char **v1909; // [esp+2B0h] [ebp-1A8h]
  char **v1910; // [esp+2B4h] [ebp-1A4h]
  char **v1911; // [esp+2B8h] [ebp-1A0h]
  char **v1912; // [esp+2BCh] [ebp-19Ch]
  int v1913; // [esp+2C0h] [ebp-198h]
  int v1914; // [esp+2C4h] [ebp-194h]
  char **v1915; // [esp+2C8h] [ebp-190h]
  char **v1916; // [esp+2CCh] [ebp-18Ch]
  char **v1917; // [esp+2D0h] [ebp-188h]
  char **v1918; // [esp+2D4h] [ebp-184h]
  int v1919; // [esp+2D8h] [ebp-180h]
  int v1920; // [esp+2DCh] [ebp-17Ch]
  char **v1921; // [esp+2E0h] [ebp-178h]
  char **v1922; // [esp+2E4h] [ebp-174h]
  char **v1923; // [esp+2E8h] [ebp-170h]
  char **v1924; // [esp+2ECh] [ebp-16Ch]
  char **v1925; // [esp+2F0h] [ebp-168h]
  char **v1926; // [esp+2F4h] [ebp-164h]
  char **v1927; // [esp+2F8h] [ebp-160h]
  char **v1928; // [esp+2FCh] [ebp-15Ch]
  int v1929; // [esp+300h] [ebp-158h]
  int v1930; // [esp+304h] [ebp-154h]
  unsigned int v1931; // [esp+308h] [ebp-150h]
  unsigned int v1932; // [esp+30Ch] [ebp-14Ch]
  unsigned int v1933; // [esp+310h] [ebp-148h]
  unsigned int v1934; // [esp+314h] [ebp-144h]
  unsigned int v1935; // [esp+318h] [ebp-140h]
  int v1936; // [esp+31Ch] [ebp-13Ch]
  int v1937; // [esp+320h] [ebp-138h]
  int v1938; // [esp+324h] [ebp-134h]
  int v1939; // [esp+328h] [ebp-130h]
  int v1940; // [esp+32Ch] [ebp-12Ch]
  int v1941; // [esp+330h] [ebp-128h]
  char **v1942; // [esp+334h] [ebp-124h]
  char **v1943; // [esp+338h] [ebp-120h]
  char **v1944; // [esp+33Ch] [ebp-11Ch]
  char **v1945; // [esp+340h] [ebp-118h]
  char **v1946; // [esp+344h] [ebp-114h]
  char **v1947; // [esp+348h] [ebp-110h]
  char **v1948; // [esp+34Ch] [ebp-10Ch]
  char **v1949; // [esp+350h] [ebp-108h]
  int v1950; // [esp+354h] [ebp-104h]
  int v1951; // [esp+358h] [ebp-100h]
  int v1952; // [esp+35Ch] [ebp-FCh]
  int v1953; // [esp+360h] [ebp-F8h]
  int v1954; // [esp+364h] [ebp-F4h]
  int v1955; // [esp+368h] [ebp-F0h]
  int v1956; // [esp+36Ch] [ebp-ECh]
  int v1957; // [esp+370h] [ebp-E8h]
  int v1958; // [esp+374h] [ebp-E4h]
  int v1959; // [esp+378h] [ebp-E0h]
  double v1960; // [esp+37Ch] [ebp-DCh]
  double v1961; // [esp+384h] [ebp-D4h]
  char **v1962; // [esp+38Ch] [ebp-CCh]
  char **v1963; // [esp+390h] [ebp-C8h]
  char **v1964; // [esp+394h] [ebp-C4h]
  char **v1965; // [esp+398h] [ebp-C0h]
  char **v1966; // [esp+39Ch] [ebp-BCh]
  char **v1967; // [esp+3A0h] [ebp-B8h]
  char **v1968; // [esp+3A4h] [ebp-B4h]
  char **v1969; // [esp+3A8h] [ebp-B0h]
  char **v1970; // [esp+3ACh] [ebp-ACh]
  char **v1971; // [esp+3B0h] [ebp-A8h]
  char **v1972; // [esp+3B4h] [ebp-A4h]
  char **v1973; // [esp+3B8h] [ebp-A0h]
  char **v1974; // [esp+3BCh] [ebp-9Ch]
  char **v1975; // [esp+3C0h] [ebp-98h]
  char **v1976; // [esp+3C4h] [ebp-94h]
  char **v1977; // [esp+3C8h] [ebp-90h]
  char **v1978; // [esp+3CCh] [ebp-8Ch]
  char **v1979; // [esp+3D0h] [ebp-88h]
  unsigned int v1980; // [esp+3D4h] [ebp-84h]
  unsigned int v1981; // [esp+3D8h] [ebp-80h]
  char **v1982; // [esp+3DCh] [ebp-7Ch]
  char **v1983; // [esp+3E0h] [ebp-78h]
  char **v1984; // [esp+3E4h] [ebp-74h]
  char **v1985; // [esp+3E8h] [ebp-70h]
  char **v1986; // [esp+3ECh] [ebp-6Ch]
  char **v1987; // [esp+3F0h] [ebp-68h]
  char **v1988; // [esp+3F4h] [ebp-64h]
  char **v1989; // [esp+3F8h] [ebp-60h]
  char **v1990; // [esp+3FCh] [ebp-5Ch]
  char **v1991; // [esp+400h] [ebp-58h]
  int v1992; // [esp+404h] [ebp-54h]
  int v1993; // [esp+408h] [ebp-50h]
  int v1994; // [esp+40Ch] [ebp-4Ch]
  int v1995; // [esp+410h] [ebp-48h]
  int v1996; // [esp+414h] [ebp-44h]
  int v1997; // [esp+418h] [ebp-40h]
  int v1998; // [esp+41Ch] [ebp-3Ch]
  int v1999; // [esp+420h] [ebp-38h]
  int *v2000; // [esp+424h] [ebp-34h]
  char *v2001; // [esp+428h] [ebp-30h]
  struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList; // [esp+42Ch] [ebp-2Ch]
  void *v2003; // [esp+430h] [ebp-28h]
  int v2004; // [esp+434h] [ebp-24h]
  int savedregs; // [esp+458h] [ebp+0h] BYREF

  _ESI = v1724; /*0x1002ea2f*/
  v2000 = &savedregs; /*0x1002ea31*/
  v6 = a1; /*0x1002ea37*/
  v2001 = v1724; /*0x1002ea39*/
  v2004 = -1; /*0x1002ea3f*/
  v2003 = &loc_1003E480; /*0x1002ea4f*/
  ExceptionList = NtCurrentTeb()->NtTib.ExceptionList; /*0x1002ea60*/
  sub_10035CD0(a1); /*0x1002ea6e*/
  sub_10035CD0(a1: v6 + 3); /*0x1002ea76*/
  v7 = v6[6]; /*0x1002ea7b*/
  v8 = v6[7]; /*0x1002ea7e*/
  i = v6; /*0x1002ea83*/
  if ( v7 != v8 ) /*0x1002ea86*/
  {
    v1730 = v8; /*0x1002ea88*/
    do /*0x1002eab4*/
    {
      v9 = *((_DWORD *)v7 + 9); /*0x1002eab6*/
      if ( v9 >= 0x10 ) /*0x1002eabc*/
      {
        v10 = *((_DWORD **)v7 + 4); /*0x1002eabe*/
        v11 = v9 + 1; /*0x1002eac1*/
        if ( v9 + 1 >= 0x1000 ) /*0x1002eaca*/
        {
          if ( (unsigned int)v10 - *(v10 - 1) - 4 >= 0x20 ) /*0x1002ead7*/
LABEL_36:
            BUG(); /*0x1002ec30*/
          v11 = v9 + 36; /*0x1002eae0*/
          v10 = (_DWORD *)*(v10 - 1); /*0x1002eae2*/
        }
        *(_DWORD *)&v1723[16] = v11; /*0x1002ea90*/
        sub_10180976(Block: v10); /*0x1002ea92*/
        v8 = v1730; /*0x1002ea9a*/
      }
      *((_DWORD *)v7 + 8) = 0; /*0x1002ea9d*/
      *((_DWORD *)v7 + 9) = 15; /*0x1002eaa4*/
      v7[16] = 0; /*0x1002eaab*/
      v7 += 44; /*0x1002eaaf*/
    }
    while ( v7 != v8 ); /*0x1002eab4*/
    v6 = i; /*0x1002eae6*/
    i[7] = i[6]; /*0x1002eaec*/
  }
  v12 = v6[9]; /*0x1002eaef*/
  v13 = v6[10]; /*0x1002eaf2*/
  if ( v12 != v13 ) /*0x1002eaf7*/
  {
    v1730 = v6[10]; /*0x1002eaf9*/
    do /*0x1002eb24*/
    {
      v14 = *((_DWORD *)v12 + 9); /*0x1002eb26*/
      if ( v14 >= 0x10 ) /*0x1002eb2c*/
      {
        v15 = *((_DWORD **)v12 + 4); /*0x1002eb2e*/
        v16 = v14 + 1; /*0x1002eb31*/
        if ( v14 + 1 >= 0x1000 ) /*0x1002eb3a*/
        {
          if ( (unsigned int)v15 - *(v15 - 1) - 4 >= 0x20 ) /*0x1002eb47*/
            goto LABEL_36; /*0x1002eb47*/
          v16 = v14 + 36; /*0x1002eb50*/
          v15 = (_DWORD *)*(v15 - 1); /*0x1002eb52*/
        }
        *(_DWORD *)&v1723[16] = v16; /*0x1002eb00*/
        sub_10180976(Block: v15); /*0x1002eb02*/
        v13 = v1730; /*0x1002eb0a*/
      }
      *((_DWORD *)v12 + 8) = 0; /*0x1002eb0d*/
      *((_DWORD *)v12 + 9) = 15; /*0x1002eb14*/
      v12[16] = 0; /*0x1002eb1b*/
      v12 += 44; /*0x1002eb1f*/
    }
    while ( v12 != v13 ); /*0x1002eb24*/
    v6 = i; /*0x1002eb56*/
    i[10] = i[9]; /*0x1002eb5c*/
  }
  v17 = v6[12]; /*0x1002eb5f*/
  v18 = v6[13]; /*0x1002eb62*/
  if ( v17 != v18 ) /*0x1002eb67*/
  {
    v1730 = v6[13]; /*0x1002eb69*/
    do /*0x1002eb94*/
    {
      v19 = *((_DWORD *)v17 + 9); /*0x1002eb96*/
      if ( v19 >= 0x10 ) /*0x1002eb9c*/
      {
        v20 = *((_DWORD **)v17 + 4); /*0x1002eb9e*/
        v21 = v19 + 1; /*0x1002eba1*/
        if ( v19 + 1 >= 0x1000 ) /*0x1002ebaa*/
        {
          if ( (unsigned int)v20 - *(v20 - 1) - 4 >= 0x20 ) /*0x1002ebb7*/
            goto LABEL_36; /*0x1002ebb7*/
          v21 = v19 + 36; /*0x1002ebbc*/
          v20 = (_DWORD *)*(v20 - 1); /*0x1002ebbe*/
        }
        *(_DWORD *)&v1723[16] = v21; /*0x1002eb70*/
        sub_10180976(Block: v20); /*0x1002eb72*/
        v18 = v1730; /*0x1002eb7a*/
      }
      *((_DWORD *)v17 + 8) = 0; /*0x1002eb7d*/
      *((_DWORD *)v17 + 9) = 15; /*0x1002eb84*/
      v17[16] = 0; /*0x1002eb8b*/
      v17 += 44; /*0x1002eb8f*/
    }
    while ( v17 != v18 ); /*0x1002eb94*/
    v6 = i; /*0x1002ebc2*/
    i[13] = i[12]; /*0x1002ebc8*/
  }
  v22 = v6[15]; /*0x1002ebcb*/
  v23 = v6[16]; /*0x1002ebce*/
  if ( v22 != v23 ) /*0x1002ebd3*/
  {
    v1730 = v6[16]; /*0x1002ebd5*/
    do /*0x1002ec0b*/
    {
      v26 = *((_DWORD *)v22 + 9); /*0x1002ec0d*/
      if ( v26 >= 0x10 ) /*0x1002ec13*/
      {
        v25 = *((_DWORD **)v22 + 4); /*0x1002ec15*/
        v24 = v26 + 1; /*0x1002ec18*/
        if ( v26 + 1 >= 0x1000 ) /*0x1002ec21*/
        {
          if ( (unsigned int)v25 - *(v25 - 1) - 4 >= 0x20 ) /*0x1002ec2e*/
            goto LABEL_36; /*0x1002ec2e*/
          v24 = v26 + 36; /*0x1002ebe3*/
          v25 = (_DWORD *)*(v25 - 1); /*0x1002ebe5*/
        }
        *(_DWORD *)&v1723[16] = v24; /*0x1002ebe7*/
        sub_10180976(Block: v25); /*0x1002ebe9*/
        v23 = v1730; /*0x1002ebf1*/
      }
      *((_DWORD *)v22 + 8) = 0; /*0x1002ebf4*/
      *((_DWORD *)v22 + 9) = 15; /*0x1002ebfb*/
      v22[16] = 0; /*0x1002ec02*/
      v22 += 44; /*0x1002ec06*/
    }
    while ( v22 != v23 ); /*0x1002ec0b*/
    v6 = i; /*0x1002ec32*/
    i[16] = i[15]; /*0x1002ec38*/
  }
  v27 = v6[18]; /*0x1002ec3b*/
  v28 = v6[19]; /*0x1002ec3e*/
  if ( v27 != v28 ) /*0x1002ec43*/
  {
    v1730 = v6[19]; /*0x1002ec45*/
    do /*0x1002ec74*/
    {
      v29 = *((_DWORD *)v27 + 9); /*0x1002ec76*/
      if ( v29 >= 0x10 ) /*0x1002ec7c*/
      {
        v30 = *((_DWORD **)v27 + 4); /*0x1002ec7e*/
        v31 = v29 + 1; /*0x1002ec81*/
        if ( v29 + 1 >= 0x1000 ) /*0x1002ec8a*/
        {
          if ( (unsigned int)v30 - *(v30 - 1) - 4 >= 0x20 ) /*0x1002ec97*/
            goto LABEL_36; /*0x1002ec97*/
          v31 = v29 + 36; /*0x1002ec9c*/
          v30 = (_DWORD *)*(v30 - 1); /*0x1002ec9e*/
        }
        *(_DWORD *)&v1723[16] = v31; /*0x1002ec50*/
        sub_10180976(Block: v30); /*0x1002ec52*/
        v28 = v1730; /*0x1002ec5a*/
      }
      *((_DWORD *)v27 + 8) = 0; /*0x1002ec5d*/
      *((_DWORD *)v27 + 9) = 15; /*0x1002ec64*/
      v27[16] = 0; /*0x1002ec6b*/
      v27 += 44; /*0x1002ec6f*/
    }
    while ( v27 != v28 ); /*0x1002ec74*/
    v6 = i; /*0x1002eca2*/
    i[19] = i[18]; /*0x1002eca8*/
  }
  v32 = v6[21]; /*0x1002ecab*/
  if ( v32 != v6[22] ) /*0x1002ecb1*/
    v6[22] = v32; /*0x1002ecb3*/
  result = (int)v6[24]; /*0x1002ecb6*/
  if ( (char *)result != v6[25] ) /*0x1002ecbc*/
    v6[25] = (char *)result; /*0x1002ecbe*/
  if ( byte_103F5396 != 0 ) /*0x1002ecc8*/
    return result; /*0x1002ecc8*/
  if ( (_BYTE)word_103F4BDD != 1 ) /*0x1002ecd5*/
    return result; /*0x1002ecd5*/
  v34 = *(int (__thiscall **)(int))(*(_DWORD *)dword_103EC8A0 + 8); /*0x1002ece3*/
  v2004 = -1; /*0x1002ece6*/
  result = v34(a1: dword_103EC8A0); /*0x1002ecf0*/
  if ( (_BYTE)result != 0 ) /*0x1002ecf4*/
    return result; /*0x1002ecf4*/
  if ( byte_103EEF74 != 0 ) /*0x1002ed01*/
    return result; /*0x1002ed01*/
  if ( byte_103F4BA8 == 1 ) /*0x1002ed0e*/
  {
    result = (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC770 + 356))(a1: dword_103EC770); /*0x1002ed18*/
    if ( (_BYTE)result != 0 ) /*0x1002ed20*/
      return result; /*0x1002ed20*/
  }
  result = sub_10179540(); /*0x1002ed2c*/
  v1748 = (char *)result; /*0x1002ed31*/
  if ( result == 0 ) /*0x1002ed39*/
    return result; /*0x1002ed39*/
  v1902 = ((double (__thiscall *)(int))*(_DWORD *)(*(_DWORD *)dword_103EC728 + 424))(a1: dword_103EC728); /*0x1002ed4d*/
  __asm /*0x1002ed53*/
  {
    vmovss xmm0, dword ptr [esi+288h]
    vmovss dword ptr [esi+284h], xmm0
  }
  v36 = word_103F4BDD; /*0x1002ed63*/
  if ( (_BYTE)word_103F4BDD == 1 ) /*0x1002ed6c*/
  {
    (*(void (__thiscall **)(int, int))(*(_DWORD *)dword_103EC728 + 420))(a1: dword_103EC728, a2: 1065353216); /*0x1002ed7f*/
    v37 = (char ***)sub_10029F30(a1: 0); /*0x1002ed8d*/
    v38 = *v37; /*0x1002ed92*/
    i = v37[1]; /*0x1002ed97*/
    while ( 1 ) /*0x1002edb3*/
    {
      if ( v38 == i ) /*0x1002edb6*/
      {
        v36 = word_103F4BDD; /*0x10030d26*/
        break; /*0x10030d26*/
      }
      if ( *v38 == nullptr ) /*0x1002edc0*/
        goto LABEL_61; /*0x1002edc0*/
      v1730 = *v38; /*0x1002edc2*/
      if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1002ede0*/
      {
        sub_101809C3(a1: &dword_103EEAD8); /*0x10030c1a*/
        if ( dword_103EEAD8 == -1 ) /*0x10030c29*/
        {
          v1743 = v38; /*0x10030c2f*/
          v2004 = 0; /*0x10030c35*/
          dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x10030c51*/
          sub_10180A43(a1: &dword_103EEAD8); /*0x10030c5b*/
          v38 = v1743; /*0x10030c63*/
        }
      }
      v39 = (wchar_t *)v1730; /*0x1002edeb*/
      v1725 = *(char **)&v1730[dword_103EEAD4]; /*0x1002edf1*/
      v40 = *(int (__thiscall **)(char *))(*(_DWORD *)v1730 + 960); /*0x1002edf6*/
      v2004 = -1; /*0x1002edfc*/
      LOBYTE(v1729) = v40(a1: v1730); /*0x1002ee0a*/
      _EAX = (*(int (__thiscall **)(wchar_t *))(*(_DWORD *)v39 + 44))(a1: v39); /*0x1002ee11*/
      __asm /*0x1002ee14*/
      {
        vmovss xmm0, dword ptr [eax]
        vmovss dword ptr [esi+40h], xmm0
        vmovss xmm0, dword ptr [eax+4]
        vmovss dword ptr [esi+1Ch], xmm0
        vmovss xmm0, dword ptr [eax+8]
        vmovss dword ptr [esi+60h], xmm0
      }
      if ( dword_103EEF7C > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1002ee4c*/
      {
        sub_101809C3(a1: &dword_103EEF7C); /*0x10030c73*/
        if ( dword_103EEF7C == -1 ) /*0x10030c82*/
        {
          v1743 = v38; /*0x10030c88*/
          v2004 = 1; /*0x10030c8e*/
          dword_103EEF78 = sub_1017C730(a1: "CTerrorPlayer", a2: "m_zombieClass"); /*0x10030caa*/
          sub_10180A43(a1: &dword_103EEF7C); /*0x10030cb4*/
          v38 = v1743; /*0x10030cbc*/
          v39 = (wchar_t *)v1730; /*0x10030cc2*/
        }
      }
      v1731 = *(_DWORD *)((char *)v39 + dword_103EEF78); /*0x1002ee5a*/
      v45 = *(void (**)(void))(*((_DWORD *)v39 + 2) + 4); /*0x1002ee63*/
      v2004 = -1; /*0x1002ee66*/
      v1728 = v39 + 4; /*0x1002ee70*/
      v45(); /*0x1002ee73*/
      if ( dword_103EEA68 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1002ee90*/
      {
        sub_101809C3(a1: &dword_103EEA68); /*0x10030ccf*/
        if ( dword_103EEA68 == -1 ) /*0x10030cde*/
        {
          v1743 = v38; /*0x10030ce4*/
          v2004 = 2; /*0x10030cea*/
          dword_103EEA64 = sub_1017C730(a1: "CBasePlayer", a2: "deadflag"); /*0x10030d06*/
          sub_10180A43(a1: &dword_103EEA68); /*0x10030d10*/
          v38 = v1743; /*0x10030d18*/
          v39 = (wchar_t *)v1730; /*0x10030d1e*/
        }
      }
      if ( *(_DWORD *)((char *)v39 + dword_103EEA64) != 0 || v39 == (wchar_t *)v1748 && dword_103F4BD8 == 0 ) /*0x1002eeb4*/
        goto LABEL_61; /*0x1002eeb4*/
      v46 = *(int (__thiscall **)(int, int))(*(_DWORD *)dword_103EC960 + 120); /*0x1002eec2*/
      v2004 = -1; /*0x1002eec5*/
      v47 = v46(a1: dword_103EC960, a2: -1); /*0x1002eed1*/
      v48 = v1730; /*0x1002eed3*/
      if ( v1730 == v1748 && v47 == 0 ) /*0x1002eee0*/
        goto LABEL_61; /*0x1002eee0*/
      v1732[0] = 0; /*0x1002eee6*/
      v1736 = 0; /*0x1002eeed*/
      v1737 = 0; /*0x1002eef4*/
      v1747 = 0; /*0x1002eefb*/
      if ( (unsigned __int8)sub_1002E000(a1: v1730, a2: v1732, a3: &v1736, a4: &v1737, a5: &v1747) == 0 ) /*0x1002ef20*/
        goto LABEL_61; /*0x1002ef20*/
      v1743 = v38; /*0x1002ef26*/
      v1733 = (char *)off_1029E57C; /*0x1002ef31*/
      v49 = *(_DWORD *)sub_10035E70(this: v48); /*0x1002ef3b*/
      v50 = (*(int (__thiscall **)(char *))(*(_DWORD *)v48 + 468))(a1: v48); /*0x1002ef41*/
      sub_10167260(a1: &v1755, a2: v50, a3: v49); /*0x1002ef53*/
      v1733 = (char *)off_1029E57C; /*0x1002ef5d*/
      v51 = *(_DWORD *)sub_10035E70(this: v48); /*0x1002ef67*/
      v52 = (*(int (__thiscall **)(char *))(*(_DWORD *)v48 + 468))(a1: v48); /*0x1002ef6d*/
      sub_101671B0(a1: &v1896, a2: v52, a3: v51); /*0x1002ef7f*/
      if ( v1725 != (char *)2 ) /*0x1002ef8a*/
      {
        v38 = v1743; /*0x1002ef93*/
        if ( v1725 != (char *)3 ) /*0x1002ef99*/
          goto LABEL_61; /*0x1002ef99*/
        if ( byte_103F4BE4 != 1 ) /*0x1002efa6*/
          goto LABEL_61; /*0x1002efa6*/
        if ( byte_103F4C22 != 1 ) /*0x1002efb3*/
          goto LABEL_61; /*0x1002efb3*/
        __asm /*0x1002efb9*/
        {
          vmovss xmm0, ds:dword_10184380
          vmulss xmm1, xmm0, dword ptr qword_103F4C74
          vmovss dword ptr [esi+110h], xmm1
          vmulss xmm1, xmm0, dword ptr qword_103F4C74+4
          vmovss dword ptr [esi+104h], xmm1
          vmulss xmm1, xmm0, dword_103F4C7C
          vmovss dword ptr [esi+0F8h], xmm1
          vmulss xmm1, xmm0, dword_103F4C28
          vmovss dword ptr [esi+108h], xmm1
          vmulss xmm1, xmm0, dword_103F4C2C
          vmovss dword ptr [esi+10Ch], xmm1
          vmulss xmm1, xmm0, dword_103F4C30
          vmovss dword ptr [esi+100h], xmm1
          vmulss xmm1, xmm0, dword_103F4C5C
          vmovss dword ptr [esi+260h], xmm1
          vmulss xmm1, xmm0, dword_103F4C60
          vmovss dword ptr [esi+0F4h], xmm1
        }
        v62 = v1748; /*0x1002f041*/
        v63 = *(_DWORD *)v1748; /*0x1002f047*/
        __asm /*0x1002f049*/
        {
          vmulss xmm1, xmm0, dword_103F4C64
          vmovss dword ptr [esi+274h], xmm1
          vmulss xmm1, xmm0, dword_103F4C88
          vmovss dword ptr [esi+278h], xmm1
          vmulss xmm1, xmm0, dword_103F4C8C
          vmovss dword ptr [esi+27Ch], xmm1
          vmulss xmm1, xmm0, dword_103F4C90
          vmovss dword ptr [esi+280h], xmm1
          vmulss xmm1, xmm0, dword ptr xmmword_103F4CE0
          vmovss dword ptr [esi+25Ch], xmm1
          vmulss xmm1, xmm0, dword ptr xmmword_103F4CE0+4
          vmovss dword ptr [esi+0ECh], xmm1
          vmulss xmm1, xmm0, dword ptr xmmword_103F4CE0+8
          vmovss dword ptr [esi+0F0h], xmm1
          vmulss xmm1, xmm0, dword_103F4CF4
          vmovss dword ptr [esi+0E4h], xmm1
          vmulss xmm1, xmm0, dword_103F4CF8
          vmovss dword ptr [esi+0E8h], xmm1
          vmulss xmm1, xmm0, dword_103F4CFC
          vmovss dword ptr [esi+0D8h], xmm1
          vmulss xmm1, xmm0, dword ptr xmmword_103F4D08
          vmovss dword ptr [esi+54h], xmm1
          vmulss xmm1, xmm0, dword ptr xmmword_103F4D08+4
          vmovss dword ptr [esi+3Ch], xmm1
          vmulss xmm0, xmm0, dword ptr xmmword_103F4D08+8
          vmovss dword ptr [esi+14h], xmm0
        }
        (*(void (__thiscall **)(char *))(v63 + 44))(a1: v62); /*0x1002f110*/
        __asm /*0x1002f113*/
        {
          vmovss xmm0, dword ptr [esi+60h]
          vsubss xmm0, xmm0, dword ptr [eax+8]
          vmovss xmm1, dword ptr [esi+1Ch]
          vsubss xmm1, xmm1, dword ptr [eax+4]
          vmovss xmm2, dword ptr [esi+40h]
          vsubss xmm2, xmm2, dword ptr [eax]
          vmulss xmm1, xmm1, xmm1
          vfmadd231ss xmm1, xmm2, xmm2
          vfmadd231ss xmm1, xmm0, xmm0
          vcvtss2sd xmm0, xmm1, xmm1
          vsqrtsd xmm0, xmm0, xmm0
          vcvtsd2ss xmm0, xmm0, xmm0
          vucomiss xmm0, dword_103F4BE0
        }
        if ( !(v89 | v90) ) /*0x1002f152*/
          goto LABEL_61; /*0x1002f152*/
        if ( ((unsigned __int8)byte_103F4BDC & (unsigned __int8)v1729) == 1 ) /*0x1002f164*/
        {
          __asm /*0x1002f16a*/
          {
            vcvttss2si eax, dword ptr [esi+110h]
            vcvttss2si ebx, dword ptr [esi+104h]
            vcvttss2si edx, dword ptr [esi+0F8h]
          }
          LOBYTE(v1755) = (_BYTE)_EAX; /*0x1002f182*/
          BYTE1(v1755) = (_BYTE)_EBX; /*0x1002f188*/
          BYTE2(v1755) = (_BYTE)_EDX; /*0x1002f18e*/
          HIBYTE(v1755) = -1; /*0x1002f194*/
          v1738 = _EAX; /*0x1002f19d*/
          v1729 = (int)_EBX; /*0x1002f1a2*/
          v1746 = (char **)_EDX; /*0x1002f1a7*/
          v1753 = _EAX; /*0x1002f1af*/
          v1754 = _EBX; /*0x1002f1b7*/
          v1752 = _EDX; /*0x1002f1bf*/
          v1901 = _EAX; /*0x1002f1c7*/
          v1757 = _EBX; /*0x1002f1cf*/
          v1756 = _EDX; /*0x1002f1d7*/
          v1760 = _EAX; /*0x1002f1df*/
          v1759 = _EBX; /*0x1002f1e7*/
          v1758 = _EDX; /*0x1002f1ef*/
          v1734 = _EAX; /*0x1002f1f5*/
          v1727 = (unsigned int)_EBX; /*0x1002f1f8*/
          v1725 = _EDX; /*0x1002f1fb*/
        }
        else
        {
          __asm /*0x1002f4d1*/
          {
            vcvttss2si eax, dword ptr [esi+108h]
            vcvttss2si ebx, dword ptr [esi+10Ch]
            vcvttss2si edx, dword ptr [esi+100h]
            vcvttss2si ecx, dword ptr [esi+260h]
          }
          v1760 = _ECX; /*0x1002f4f1*/
          __asm { vcvttss2si ecx, dword ptr [esi+0F4h] } /*0x1002f4f7*/
          v1759 = _ECX; /*0x1002f4ff*/
          __asm { vcvttss2si ecx, dword ptr [esi+274h] } /*0x1002f505*/
          v1758 = _ECX; /*0x1002f50d*/
          __asm { vcvttss2si ecx, dword ptr [esi+278h] } /*0x1002f513*/
          v1901 = _ECX; /*0x1002f51b*/
          __asm { vcvttss2si ecx, dword ptr [esi+27Ch] } /*0x1002f521*/
          v1757 = _ECX; /*0x1002f529*/
          __asm { vcvttss2si ecx, dword ptr [esi+280h] } /*0x1002f52f*/
          v1756 = _ECX; /*0x1002f537*/
          __asm { vcvttss2si ecx, dword ptr [esi+25Ch] } /*0x1002f53d*/
          v1753 = _ECX; /*0x1002f545*/
          __asm { vcvttss2si ecx, dword ptr [esi+0ECh] } /*0x1002f54b*/
          v1754 = _ECX; /*0x1002f553*/
          __asm { vcvttss2si ecx, dword ptr [esi+0F0h] } /*0x1002f559*/
          v1752 = _ECX; /*0x1002f561*/
          __asm { vcvttss2si ecx, dword ptr [esi+0E4h] } /*0x1002f567*/
          v1738 = _ECX; /*0x1002f56f*/
          __asm { vcvttss2si ecx, dword ptr [esi+0E8h] } /*0x1002f572*/
          v1729 = _ECX; /*0x1002f57a*/
          __asm { vcvttss2si ecx, dword ptr [esi+0D8h] } /*0x1002f57d*/
          v1746 = _ECX; /*0x1002f585*/
          __asm { vcvttss2si ecx, dword ptr [esi+54h] } /*0x1002f58b*/
          v1734 = (wchar_t *)_ECX; /*0x1002f590*/
          __asm { vcvttss2si ecx, dword ptr [esi+3Ch] } /*0x1002f593*/
          v1727 = _ECX; /*0x1002f598*/
          __asm { vcvttss2si ecx, dword ptr [esi+14h] } /*0x1002f59b*/
          v1725 = _ECX; /*0x1002f5a0*/
        }
        if ( byte_103F4C23 == 1 ) /*0x1002f5aa*/
        {
          if ( dword_103F4C24 == 1 ) /*0x1002f5b9*/
          {
            v1888 = (char)_EAX; /*0x1002f727*/
            v1889 = (char)_EBX; /*0x1002f72d*/
            __asm { vcvttss2si eax, dword ptr [esi+0DCh] } /*0x1002f733*/
            v1733 = _EAX; /*0x1002f73b*/
            v1890 = (char)_EDX; /*0x1002f73e*/
            __asm { vcvttss2si eax, dword ptr [esi+5Ch] } /*0x1002f744*/
            v1735 = _EAX; /*0x1002f749*/
            v1891 = -1; /*0x1002f74c*/
            __asm { vcvttss2si edi, dword ptr [esi+58h] } /*0x1002f753*/
            __asm { vcvttss2si edx, dword ptr [esi+34h] }
            sub_101778E0(a1: _EDX, a2: _EDI, a3: _EAX, a4: v1733, a5: v1888, a6: dword_103F4C38); /*0x1002f773*/
          }
          else if ( dword_103F4C24 == 0 ) /*0x1002f5c1*/
          {
            v172 = dword_103F4C38; /*0x1002f5c7*/
            v1892 = (char)_EAX; /*0x1002f5cd*/
            v1893 = (char)_EBX; /*0x1002f5d9*/
            v1894 = (char)_EDX; /*0x1002f5df*/
            v1895 = -1; /*0x1002f5e5*/
            __asm /*0x1002f5ec*/
            {
              vmovss xmm0, dword ptr [esi+0DCh]
              vmovss dword ptr [esi+54h], xmm0
              vcvttss2si eax, xmm0
            }
            v1751 = _EAX; /*0x1002f5fd*/
            __asm /*0x1002f603*/
            {
              vmovss xmm0, dword ptr [esi+5Ch]
              vmovss dword ptr [esi+0E8h], xmm0
              vcvttss2si eax, xmm0
            }
            v1900 = _EAX; /*0x1002f614*/
            __asm /*0x1002f61a*/
            {
              vmovss xmm1, dword ptr [esi+58h]
              vmovss dword ptr [esi+0E4h], xmm1
              vmovss xmm0, dword ptr [esi+34h]
              vmovss dword ptr [esi+3Ch], xmm0
              vcvttss2si ebx, xmm1
              vcvttss2si edx, xmm0
            }
            sub_10177840(a1: _EDX, a2: _EBX, a3: v1900, a4: v1751, a5: v1892, a6: v172); /*0x1002f64f*/
            __asm /*0x1002f657*/
            {
              vcvtsi2ss xmm0, xmm3, eax
              vaddss xmm1, xmm0, dword ptr [esi+54h]
              vcvttss2si eax, xmm1
            }
            v1751 = _EAX; /*0x1002f664*/
            __asm /*0x1002f66a*/
            {
              vaddss xmm0, xmm0, dword ptr [esi+0E8h]
              vcvttss2si edx, xmm0
              vcvtsi2ss xmm0, xmm3, edi
            }
            __asm
            {
              vmovss xmm1, dword ptr [esi+0E4h]
              vsubss xmm1, xmm1, xmm0
              vcvttss2si edi, xmm1
            }
            v1959 = -16777216; /*0x1002f690*/
            __asm /*0x1002f69a*/
            {
              vmovss xmm1, dword ptr [esi+3Ch]
              vsubss xmm0, xmm1, xmm0
              vcvttss2si ebx, xmm0
            }
            sub_10177840(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: 0, a6: 1); /*0x1002f6b9*/
            __asm /*0x1002f6be*/
            {
              vmovss xmm1, ds:dword_10185850
              vaddss xmm0, xmm1, dword ptr [esi+54h]
              vcvttss2si eax, xmm0
            }
            v1735 = _EAX; /*0x1002f6cf*/
            __asm /*0x1002f6d2*/
            {
              vaddss xmm0, xmm1, dword ptr [esi+0E8h]
              vcvttss2si edx, xmm0
            }
            __asm
            {
              vmovss xmm1, ds:dword_10184184
              vaddss xmm0, xmm1, dword ptr [esi+0E4h]
              vcvttss2si edi, xmm0
            }
            v1958 = -16777216; /*0x1002f6f8*/
            __asm /*0x1002f702*/
            {
              vaddss xmm0, xmm1, dword ptr [esi+3Ch]
              vcvttss2si ebx, xmm0
            }
            sub_10177840(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: 0, a6: 1); /*0x1002f71a*/
          }
        }
        v206 = 0; /*0x1002f778*/
        if ( byte_103F4C3C == 1 ) /*0x1002f781*/
        {
          if ( dword_103F4C40 == 1 ) /*0x1002f78f*/
          {
            __asm /*0x1002f93e*/
            {
              vxorps xmm0, xmm0, xmm0
              vmovups ymmword ptr [esi+230h], ymm0
              vmovaps ymmword ptr [esi+220h], ymm0
              vmovaps ymmword ptr [esi+200h], ymm0
              vmovaps ymmword ptr [esi+1E0h], ymm0
              vmovaps ymmword ptr [esi+1C0h], ymm0
            }
            v223 = dword_103EC770; /*0x1002f96a*/
            __asm { vzeroupper } /*0x1002f975*/
            v224 = (*(int (__thiscall **)(wchar_t *))(*(_DWORD *)v1728 + 32))(a1: v1728); /*0x1002f978*/
            v206 = 0; /*0x1002f98a*/
            if ( (*(unsigned __int8 (__thiscall **)(int, int, float **))(*(_DWORD *)v223 + 32))( /*0x1002f98e*/
                   a1: v223,
                   a2: v224,
                   a3: &v1897) != 0 )
            {
              __asm /*0x1002f994*/
              {
                vmovss xmm0, dword ptr [esi+34h]
                vaddss xmm0, xmm0, dword ptr [esi+5Ch]
                vaddss xmm0, xmm0, ds:dword_10185854
                vmovss dword ptr [esi+54h], xmm0
                vxorps xmm0, xmm0, xmm0
                vaddss xmm0, xmm0, dword ptr [esi+58h]
                vmovss dword ptr [esi+3Ch], xmm0
              }
              sub_1017B960(a1: v1744); /*0x1002f9c6*/
              v1728 = *(wchar_t **)ArgList; /*0x1002f9d0*/
              sub_1000E5E0(a1: v1741, Str: (char *)v1899); /*0x1002f9e2*/
              v2004 = 5; /*0x1002f9e7*/
              sub_1002A600(lpWideCharStr: (LPWSTR)v1744, lpMultiByteStr: (LPCCH)v1741); /*0x1002f9f5*/
              v1880 = (char)v1760; /*0x1002fa00*/
              v1750 = (wchar_t *)v1744[0]; /*0x1002fa0c*/
              v1881 = (char)v1759; /*0x1002fa18*/
              v1749 = v1745; /*0x1002fa24*/
              v1882 = (char)v1758; /*0x1002fa30*/
              v1883 = -1; /*0x1002fa36*/
              v2004 = 6; /*0x1002fa43*/
              sub_1017B960(a1: &v1739); /*0x1002fa51*/
              __asm { vcvttss2si eax, dword ptr [esi+54h] } /*0x1002fa56*/
              __asm { vcvttss2si ecx, dword ptr [esi+3Ch] }
              v232 = v1750; /*0x1002fa6d*/
              if ( v1749 < 8 ) /*0x1002fa73*/
                v232 = (wchar_t *)v1744; /*0x1002fa73*/
              sub_10177410( /*0x1002fa89*/
                a1: (int)v1728,
                a2: (int)&v1739,
                a3: _EAX,
                a4: _ECX,
                a5: v1880,
                a6: 1,
                Format: v232,
                ArgList: v1724[0]);
              sub_1000BCE0(a1: v1744); /*0x1002fa97*/
              sub_1000BC90(a1: v1741); /*0x1002faa2*/
              v206 = 1; /*0x1002faa7*/
            }
          }
          else if ( dword_103F4C40 == 0 ) /*0x1002f797*/
          {
            __asm /*0x1002f79d*/
            {
              vxorps xmm0, xmm0, xmm0
              vmovups ymmword ptr [esi+230h], ymm0
              vmovaps ymmword ptr [esi+220h], ymm0
              vmovaps ymmword ptr [esi+200h], ymm0
              vmovaps ymmword ptr [esi+1E0h], ymm0
              vmovaps ymmword ptr [esi+1C0h], ymm0
            }
            v208 = dword_103EC770; /*0x1002f7c9*/
            __asm { vzeroupper } /*0x1002f7d4*/
            v209 = (*(int (__thiscall **)(wchar_t *))(*(_DWORD *)v1728 + 32))(a1: v1728); /*0x1002f7d7*/
            if ( (*(unsigned __int8 (__thiscall **)(int, int, float **))(*(_DWORD *)v208 + 32))( /*0x1002f7eb*/
                   a1: v208,
                   a2: v209,
                   a3: &v1897) != 0 )
            {
              v1733 = *(char **)ArgList; /*0x1002f7f6*/
              sub_1000E5E0(a1: v1741, Str: (char *)v1899); /*0x1002f808*/
              v2004 = 3; /*0x1002f80d*/
              sub_1002A600(lpWideCharStr: (LPWSTR)v1744, lpMultiByteStr: (LPCCH)v1741); /*0x1002f81f*/
              v1728 = (wchar_t *)v1744; /*0x1002f831*/
              if ( v1745 >= 8 ) /*0x1002f834*/
                v1728 = (wchar_t *)v1744[0]; /*0x1002f83c*/
              v1884 = (char)v1760; /*0x1002f845*/
              v1885 = (char)v1759; /*0x1002f851*/
              v1886 = (char)v1758; /*0x1002f85d*/
              v1887 = -1; /*0x1002f863*/
              __asm /*0x1002f86a*/
              {
                vmovss xmm0, dword ptr [esi+58h]
                vmovss dword ptr [esi+54h], xmm0
              }
              v2004 = 4; /*0x1002f87a*/
              sub_1017B960(a1: &v1739); /*0x1002f888*/
              v1750 = (wchar_t *)v1740; /*0x1002f893*/
              __asm /*0x1002f899*/
              {
                vmovss xmm0, dword ptr [esi+34h]
                vmovss dword ptr [esi+0E4h], xmm0
                vmovss xmm0, dword ptr [esi+5Ch]
                vmovss dword ptr [esi+0ECh], xmm0
              }
              sub_1017B960(a1: v1742); /*0x1002f8c0*/
              __asm /*0x1002f8c5*/
              {
                vmovss xmm0, dword ptr [esi+0ECh]
                vmulss xmm0, xmm0, ds:dword_10184EDC
                vaddss xmm0, xmm0, dword ptr [esi+0E4h]
                vcvttss2si eax, xmm0
              }
              __asm
              {
                vcvtsi2ss xmm0, xmm3, ecx
                vmovss xmm1, dword ptr [esi+54h]
                vsubss xmm0, xmm1, xmm0
                vaddss xmm0, xmm0, ds:dword_10185850
                vcvttss2si ecx, xmm0
              }
              sub_10177410( /*0x1002f919*/
                a1: (int)v1733,
                a2: (int)v1742,
                a3: _EAX,
                a4: _ECX,
                a5: v1884,
                a6: 8,
                Format: v1728,
                ArgList: v1724[0]);
              sub_1000BCE0(a1: v1744); /*0x1002f927*/
              sub_1000BC90(a1: v1741); /*0x1002f932*/
              v206 = 0; /*0x1002f937*/
            }
          }
        }
        v233 = v1730; /*0x1002faac*/
        if ( byte_103F4C6C == 1 ) /*0x1002fab6*/
        {
          if ( dword_103F4C70 != 0 ) /*0x1002fac3*/
          {
            if ( dword_103F4C70 == 1 ) /*0x1002facc*/
            {
              v1728 = (wchar_t *)"Bot"; /*0x1002fad8*/
              if ( (unsigned int)(v1731 - 1) <= 7 ) /*0x1002fae2*/
              {
                v1728 = (wchar_t *)dword_1018581C[v1731]; /*0x1002faeb*/
                v2004 = -1; /*0x1002faee*/
              }
              v1731 = *(_DWORD *)ArgList; /*0x1002fafd*/
              v1876 = (char)v1901; /*0x1002fb06*/
              v1877 = (char)v1757; /*0x1002fb12*/
              v1878 = (char)v1756; /*0x1002fb1e*/
              v1879 = -1; /*0x1002fb24*/
              __asm /*0x1002fb2b*/
              {
                vmovss xmm0, dword ptr [esi+58h]
                vmovss dword ptr [esi+54h], xmm0
              }
              v2004 = -1; /*0x1002fb3b*/
              sub_1017B960(a1: &v1897); /*0x1002fb4c*/
              v1733 = (char *)(v206 + 1); /*0x1002fb54*/
              __asm /*0x1002fb5e*/
              {
                vcvtsi2ss xmm0, xmm3, ebx
                vaddss xmm0, xmm0, dword ptr [esi+54h]
                vcvttss2si edi, xmm0
                vmovss xmm0, dword ptr [esi+34h]
                vaddss xmm0, xmm0, dword ptr [esi+5Ch]
                vaddss xmm0, xmm0, ds:dword_10185854
                vcvttss2si ebx, xmm0
              }
              sub_1017B960(a1: v1744); /*0x1002fb8e*/
              sub_10177250( /*0x1002fbb0*/
                a1: v1731,
                a2: (int)v1744,
                a3: _EBX,
                a4: _EDI,
                a5: v1876,
                a6: 1,
                Format: "%hs",
                ArgList: (char)v1728);
              v233 = v1730; /*0x1002fbbb*/
            }
          }
          else
          {
            v1728 = (wchar_t *)v206; /*0x10030000*/
            v285 = *(unsigned __int8 (**)(void))(*(_DWORD *)v1730 + 960); /*0x10030005*/
            v2004 = -1; /*0x1003000b*/
            if ( v285() != 0 ) /*0x1003001f*/
              v286 = sub_10167A60(a1: v1731); /*0x10030028*/
            else
              v286 = sub_101672D0(a1: v1731); /*0x10030204*/
            v1731 = v286; /*0x10030209*/
            v321 = (*(int (__thiscall **)(char *))(*(_DWORD *)v1730 + 960))(a1: v1730); /*0x10030211*/
            LOBYTE(_ECX) = -1; /*0x10030217*/
            LOBYTE(_EDX) = -1; /*0x10030219*/
            LOBYTE(_EBX) = -1; /*0x1003021b*/
            if ( v321 != 0 ) /*0x1003021f*/
            {
              __asm /*0x10030221*/
              {
                vmovss xmm1, ds:dword_10184380
                vmulss xmm0, xmm1, dword ptr qword_103F4C74
                vcvttss2si ecx, xmm0
                vmulss xmm0, xmm1, dword ptr qword_103F4C74+4
                vcvttss2si edx, xmm0
                vmulss xmm0, xmm1, dword_103F4C7C
                vcvttss2si ebx, xmm0
              }
            }
            v1733 = *(char **)ArgList; /*0x10030252*/
            v1872 = _ECX; /*0x10030255*/
            v1873 = _EDX; /*0x1003025b*/
            v1874 = _EBX; /*0x10030261*/
            v1875 = -1; /*0x10030267*/
            v329 = dword_103F4C84; /*0x1003026e*/
            __asm /*0x10030274*/
            {
              vmovss xmm0, dword ptr [esi+58h]
              vmovss dword ptr [esi+54h], xmm0
            }
            sub_1017B960(a1: &v1897); /*0x1003028b*/
            __asm /*0x10030297*/
            {
              vcvtsi2ss xmm0, xmm3, eax
              vmovss xmm1, dword ptr [esi+54h]
              vsubss xmm0, xmm1, xmm0
              vaddss xmm0, xmm0, ds:dword_10185850
              vaddss xmm0, xmm0, ds:dword_10185880
              vcvttss2si eax, xmm0
              vmovss xmm0, ds:dword_10184EDC
              vmulss xmm0, xmm0, dword ptr [esi+5Ch]
              vaddss xmm0, xmm0, dword ptr [esi+34h]
              vcvttss2si edx, xmm0
            }
            sub_10178600(a1: _EDX, a2: _EAX, a3: v329, a4: v329, a5: v1731, a6: 8, a7: v1872); /*0x100302e1*/
            v233 = v1730; /*0x100302e6*/
          }
        }
        if ( byte_103F4C98 == 1 ) /*0x100302f3*/
        {
          if ( dword_103F4C9C == 1 ) /*0x10030301*/
          {
            v1728 = *(wchar_t **)ArgList; /*0x100303b8*/
            v350 = *(int (__thiscall **)(char *))(*(_DWORD *)v233 + 468); /*0x100303bd*/
            v2004 = -1; /*0x100303c3*/
            v1731 = v350(a1: v1730); /*0x100303d2*/
            __asm /*0x100303db*/
            {
              vmovss xmm0, dword ptr [esi+58h]
              vaddss xmm0, xmm0, dword ptr [esi+0DCh]
              vmovss xmm1, ds:dword_10185854
              vaddss xmm0, xmm0, xmm1
              vaddss xmm0, xmm0, xmm1
              vaddss xmm0, xmm0, xmm1
              vcvttss2si ebx, xmm0
              vmovss xmm0, ds:dword_10184EDC
              vmulss xmm0, xmm0, dword ptr [esi+5Ch]
              vaddss xmm0, xmm0, dword ptr [esi+34h]
            }
            v1956 = v1755; /*0x10030412*/
            __asm { vcvttss2si edi, xmm0 } /*0x10030418*/
            sub_1017B960(a1: &v1897); /*0x10030429*/
            sub_10177250( /*0x1003044b*/
              a1: (int)v1728,
              a2: (int)&v1897,
              a3: _EDI,
              a4: _EBX,
              a5: v1956,
              a6: 8,
              Format: (char *)"%d",
              ArgList: v1731);
LABEL_148:
            v233 = v1730; /*0x10030450*/
          }
          else if ( dword_103F4C9C == 0 ) /*0x10030309*/
          {
            v1728 = *(wchar_t **)ArgList; /*0x10030314*/
            v341 = *(int (__thiscall **)(char *))(*(_DWORD *)v233 + 468); /*0x10030319*/
            v2004 = -1; /*0x1003031f*/
            v1731 = v341(a1: v1730); /*0x1003032e*/
            v1957 = v1755; /*0x10030337*/
            __asm /*0x1003033d*/
            {
              vmovss xmm0, dword ptr [esi+58h]
              vmovss dword ptr [esi+3Ch], xmm0
            }
            sub_1017B960(a1: &v1897); /*0x10030354*/
            __asm /*0x10030360*/
            {
              vcvtsi2ss xmm0, xmm3, ebx
              vaddss xmm0, xmm0, dword ptr [esi+3Ch]
              vcvttss2si ebx, xmm0
              vmovss xmm0, dword ptr [esi+34h]
              vaddss xmm0, xmm0, dword ptr [esi+5Ch]
              vaddss xmm0, xmm0, ds:dword_10185854
              vcvttss2si edi, xmm0
            }
            sub_1017B960(a1: v1744); /*0x10030390*/
            sub_10177250( /*0x100303ae*/
              a1: (int)v1728,
              a2: (int)v1744,
              a3: _EDI,
              a4: _EBX,
              a5: v1957,
              a6: 1,
              Format: (char *)"%d",
              ArgList: v1731);
            goto LABEL_148; /*0x100303ae*/
          }
        }
        if ( byte_103F4CB8 == 1 ) /*0x1003045d*/
        {
          if ( dword_103F4CBC == 1 ) /*0x1003046b*/
          {
            v372 = *(int (__thiscall **)(char *))(*(_DWORD *)v233 + 468); /*0x100304f9*/
            v2004 = -1; /*0x100304ff*/
            v373 = v372(a1: v1730); /*0x1003050e*/
            v374 = (v373 != 0) & (*(_DWORD *)sub_10035E70(this: v1730) != 0); /*0x10030524*/
            v233 = v1730; /*0x10030526*/
            if ( v374 == 1 ) /*0x1003052c*/
            {
              __asm /*0x10030532*/
              {
                vcvtsi2ss xmm2, xmm3, edi
                vcvtsi2ss xmm0, xmm3, eax
                vmaxss xmm3, xmm2, xmm0
                vmovss xmm0, dword ptr [esi+58h]
                vaddss xmm0, xmm0, dword ptr [esi+0DCh]
                vaddss xmm0, xmm0, ds:dword_10185854
                vmovss xmm1, dword ptr [esi+5Ch]
                vcvttps2dq xmm4, xmm1
                vcvtdq2ps xmm4, xmm4
                vucomiss xmm3, ds:dword_10184F30
              }
              if ( __SETP__(1, 1) ) /*0x1003056e*/
              {
                __asm /*0x100306d2*/
                {
                  vdivss xmm2, xmm2, xmm3
                  vxorps xmm6, xmm6, xmm6
                  vmaxss xmm3, xmm6, xmm2
                  vmovss xmm5, ds:dword_10184184
                  vcmpltss xmm2, xmm5, xmm2
                  vbroadcastss xmm5, ds:dword_10184184
                  vblendvps xmm2, xmm3, xmm5, xmm2
                  vfmadd213ss xmm2, xmm4, xmm6
                }
              }
              else
              {
                __asm /*0x10030574*/
                {
                  vcmpless xmm2, xmm3, xmm2
                  vandps xmm2, xmm2, xmm4
                }
              }
              __asm /*0x100306ff*/
              {
                vcvttss2si ebx, xmm0
                vcvttss2si eax, xmm1
                vcvttss2si ecx, xmm2
              }
              v1728 = _ECX; /*0x1003070b*/
              v1997 = -16119286; /*0x10030714*/
              __asm /*0x10030721*/
              {
                vmovss xmm0, dword ptr [esi+34h]
                vmovss dword ptr [esi+30h], xmm0
                vaddss xmm0, xmm0, ds:dword_1018555C
              }
              __asm { vcvttss2si edi, xmm0 }
              sub_101777E0(a1: _EDI, a2: _EBX - 1, a3: _EAX + 2, a4: 4, a5: -16119286); /*0x10030744*/
              v1996 = v1755; /*0x10030755*/
              __asm { vcvttss2si edx, dword ptr [esi+30h] } /*0x1003075b*/
              *(_DWORD *)&v1723[16] = v1755; /*0x10030760*/
              *(_DWORD *)&v1723[12] = 2; /*0x10030761*/
              *(_DWORD *)&v1723[8] = v1728; /*0x10030763*/
              *(_DWORD *)&v1723[4] = _EBX; /*0x10030766*/
              *(_DWORD *)v1723 = _EDX; /*0x10030767*/
LABEL_164:
              sub_10177780( /*0x10030768*/
                a1: *(_DWORD *)v1723,
                a2: *(_DWORD *)&v1723[4],
                a3: *(_DWORD *)&v1723[8],
                a4: *(_DWORD *)&v1723[12],
                a5: *(_DWORD *)&v1723[16]);
              v233 = v1730; /*0x1003076d*/
            }
          }
          else if ( dword_103F4CBC == 0 ) /*0x10030473*/
          {
            v362 = *(int (__thiscall **)(char *))(*(_DWORD *)v233 + 468); /*0x1003047b*/
            v2004 = -1; /*0x10030481*/
            v363 = v362(a1: v1730); /*0x10030490*/
            v364 = (v363 != 0) & (*(_DWORD *)sub_10035E70(this: v1730) != 0); /*0x100304a6*/
            v233 = v1730; /*0x100304a8*/
            if ( v364 == 1 ) /*0x100304ae*/
            {
              __asm /*0x100304b4*/
              {
                vcvtsi2ss xmm2, xmm3, edi
                vcvtsi2ss xmm0, xmm3, eax
                vmovss xmm1, ds:dword_10185858
                vaddss xmm1, xmm1, dword ptr [esi+34h]
                vmaxss xmm3, xmm2, xmm0
                vmovss xmm0, dword ptr [esi+0DCh]
                vucomiss xmm3, ds:dword_10184F30
              }
              if ( __SETP__(1, 1) ) /*0x100304e3*/
              {
                __asm /*0x10030582*/
                {
                  vdivss xmm2, xmm2, xmm3
                  vxorps xmm5, xmm5, xmm5
                  vmaxss xmm3, xmm5, xmm2
                  vmovss xmm4, ds:dword_10184184
                  vcmpltss xmm2, xmm4, xmm2
                  vbroadcastss xmm4, ds:dword_10184184
                  vblendvps xmm2, xmm3, xmm4, xmm2
                  vfmadd213ss xmm2, xmm0, xmm5
                }
              }
              else
              {
                __asm /*0x100304e9*/
                {
                  vcmpless xmm2, xmm3, xmm2
                  vandps xmm2, xmm2, xmm0
                }
              }
              __asm { vcvttss2si eax, xmm1 } /*0x100305af*/
              v1731 = _EAX; /*0x100305b3*/
              __asm { vcvttss2si edx, xmm2 } /*0x100305b6*/
              v1728 = _EDX; /*0x100305ba*/
              v1999 = -16119286; /*0x100305c3*/
              __asm { vaddss xmm0, xmm0, dword ptr [esi+58h] } /*0x100305cd*/
              v391 = _EDX + 1; /*0x100305d2*/
              __asm /*0x100305d5*/
              {
                vcvttps2dq xmm1, xmm2
                vcvtdq2ps xmm1, xmm1
                vsubss xmm0, xmm0, xmm1
                vmovss dword ptr [esi+3Ch], xmm0
                vaddss xmm0, xmm0, ds:dword_1018555C
                vcvttss2si edx, xmm0
              }
              sub_101777E0(a1: _EAX - 1, a2: _EDX, a3: 4, a4: v391, a5: -16119286); /*0x100305ff*/
              v1998 = v1755; /*0x10030610*/
              __asm { vcvttss2si edx, dword ptr [esi+3Ch] } /*0x10030616*/
              *(_DWORD *)&v1723[16] = v1755; /*0x1003061b*/
              *(_DWORD *)&v1723[12] = v1728; /*0x1003061c*/
              *(_DWORD *)&v1723[8] = 2; /*0x1003061f*/
              *(_DWORD *)&v1723[4] = _EDX; /*0x10030621*/
              *(_DWORD *)v1723 = v1731; /*0x10030622*/
              goto LABEL_164; /*0x10030625*/
            }
          }
        }
        if ( v233 != v1748 && byte_103F4CD8 != 0 ) /*0x10030783*/
        {
          LOBYTE(v1871) = (_BYTE)v1753; /*0x10030795*/
          BYTE1(v1871) = (_BYTE)v1754; /*0x100307a1*/
          BYTE2(v1871) = (_BYTE)v1752; /*0x100307ad*/
          HIBYTE(v1871) = -1; /*0x100307b3*/
          __asm { vmovss xmm0, dword ptr [esi+58h] } /*0x100307bf*/
          if ( dword_103F4CDC == 2 ) /*0x100307c7*/
          {
            __asm /*0x100307ea*/
            {
              vmovss xmm2, ds:dword_10184EDC
              vmulss xmm1, xmm2, dword ptr [esi+0DCh]
              vaddss xmm0, xmm0, xmm1
              vmulss xmm1, xmm2, dword ptr [esi+5Ch]
              vaddss xmm1, xmm1, dword ptr [esi+34h]
            }
            v430 = *(_DWORD *)(*(_DWORD *)ArgList + 4) / 2; /*0x10030812*/
            goto LABEL_173; /*0x10030814*/
          }
          if ( dword_103F4CDC == 1 ) /*0x100307cc*/
          {
            __asm /*0x100307ce*/
            {
              vaddss xmm0, xmm0, dword ptr [esi+0DCh]
              vmovss xmm1, ds:dword_10184EDC
              vmulss xmm1, xmm1, dword ptr [esi+5Ch]
              vaddss xmm1, xmm1, dword ptr [esi+34h]
            }
            goto LABEL_172; /*0x100307e8*/
          }
          __asm /*0x10030816*/
          {
            vmovss xmm1, ds:dword_10184EDC
            vmulss xmm1, xmm1, dword ptr [esi+5Ch]
            vaddss xmm1, xmm1, dword ptr [esi+34h]
          }
          if ( dword_103F4CDC != 0 ) /*0x1003082a*/
          {
LABEL_172:
            v430 = *(_DWORD *)(*(_DWORD *)ArgList + 4); /*0x10030830*/
LABEL_173:
            v2004 = -1; /*0x10030833*/
          }
          else
          {
            v430 = 0; /*0x1003095b*/
          }
          __asm { vcvttss2si eax, xmm0 } /*0x1003083d*/
          v1728 = _EAX; /*0x10030841*/
          __asm { vcvttss2si ebx, xmm1 } /*0x1003084d*/
          v435 = **(_DWORD **)ArgList / 2; /*0x10030851*/
          v2004 = -1; /*0x10030853*/
          *(_DWORD *)&v1723[12] = _EAX; /*0x10030864*/
          *(_DWORD *)&v1723[8] = _EBX; /*0x10030867*/
          *(_DWORD *)&v1723[4] = v430; /*0x10030868*/
          *(_DWORD *)v1723 = v435; /*0x10030869*/
          sub_101775B0(a1: *(__int128 *)v1723, a2: v1871); /*0x1003086a*/
          v233 = v1730; /*0x1003086f*/
        }
        v38 = v1743; /*0x10030879*/
        if ( byte_103F4CF0 == 1 ) /*0x1003087f*/
        {
          v1867 = (char)v1738; /*0x10030884*/
          v1868 = v1729; /*0x1003088d*/
          v1869 = (char)v1746; /*0x10030899*/
          v1870 = -1; /*0x1003089f*/
          v2004 = -1; /*0x100308a6*/
          sub_1002E810(a1: v233, a2: (char)v1738); /*0x100308b8*/
        }
        if ( byte_103F4D04 == 1 ) /*0x100308c4*/
        {
          __asm /*0x100308ca*/
          {
            vmovss xmm0, dword ptr [esi+34h]
            vmovss xmm1, ds:dword_10184EDC
            vmulss xmm1, xmm1, dword ptr [esi+5Ch]
            vmovss xmm2, ds:dword_10185850
            vaddss xmm2, xmm2, dword ptr [esi+58h]
            vbroadcastss xmm2, xmm2
            vblendps xmm1, xmm1, xmm2, 2
            vblendps xmm0, xmm0, ds:xmmword_10185860, 0Eh
            vaddps xmm0, xmm0, xmm1
            vmovddup xmm1, xmm0
            vaddps xmm1, xmm1, ds:xmmword_10185870
            vmovups xmmword ptr [esi+1C0h], xmm1
            vmovlps qword ptr [esi+1D0h], xmm0
          }
          LOBYTE(v1866) = (_BYTE)v1734; /*0x10030927*/
          BYTE1(v1866) = v1727; /*0x10030930*/
          BYTE2(v1866) = (_BYTE)v1725; /*0x10030939*/
          HIBYTE(v1866) = -1; /*0x1003093f*/
          v2004 = -1; /*0x10030946*/
          v447 = v1866; /*0x10030950*/
LABEL_60:
          sub_10178F50(a1: &v1897, a2: v447); /*0x1002ed9c*/
          goto LABEL_61; /*0x1002eda4*/
        }
        goto LABEL_61; /*0x100308c4*/
      }
      v38 = v1743; /*0x1002f20a*/
      if ( byte_103F4BE5 != 1 ) /*0x1002f210*/
        goto LABEL_61; /*0x1002f210*/
      if ( byte_103F4D18 != 1 ) /*0x1002f21d*/
        goto LABEL_61; /*0x1002f21d*/
      __asm /*0x1002f223*/
      {
        vmovss xmm0, ds:dword_10184380
        vmulss xmm1, xmm0, dword_103F4D20
        vmovss dword ptr [esi+0D8h], xmm1
        vmulss xmm1, xmm0, dword_103F4D24
        vmovss dword ptr [esi+54h], xmm1
        vmulss xmm1, xmm0, dword_103F4D28
        vmovss dword ptr [esi+3Ch], xmm1
        vmulss xmm1, xmm0, dword_103F4D54
        vmovss dword ptr [esi+30h], xmm1
        vmulss xmm1, xmm0, dword_103F4D58
        vmovss dword ptr [esi+24h], xmm1
        vmulss xmm1, xmm0, dword_103F4D5C
        vmovss dword ptr [esi+14h], xmm1
        vmulss xmm1, xmm0, dword ptr xmmword_103F4DAC
        vmovss dword ptr [esi+104h], xmm1
        vmulss xmm1, xmm0, dword ptr xmmword_103F4DAC+4
        vmovss dword ptr [esi+0ECh], xmm1
      }
      v103 = v1748; /*0x1002f29c*/
      v104 = *(_DWORD *)v1748; /*0x1002f2a2*/
      __asm /*0x1002f2a4*/
      {
        vmulss xmm1, xmm0, dword ptr xmmword_103F4DAC+8
        vmovss dword ptr [esi+100h], xmm1
        vmulss xmm1, xmm0, dword_103F4DC0
        vmovss dword ptr [esi+260h], xmm1
        vmulss xmm1, xmm0, dword_103F4DC4
        vmovss dword ptr [esi+0F8h], xmm1
        vmulss xmm1, xmm0, dword_103F4DC8
        vmovss dword ptr [esi+0F0h], xmm1
        vmulss xmm1, xmm0, dword ptr xmmword_103F4DD4
        vmovss dword ptr [esi+0F4h], xmm1
        vmulss xmm1, xmm0, dword ptr xmmword_103F4DD4+4
        vmovss dword ptr [esi+0E4h], xmm1
        vmulss xmm0, xmm0, dword ptr xmmword_103F4DD4+8
        vmovss dword ptr [esi+0E8h], xmm0
      }
      (*(void (__thiscall **)(char *))(v104 + 44))(a1: v103); /*0x1002f314*/
      __asm /*0x1002f317*/
      {
        vmovss xmm0, dword ptr [esi+60h]
        vsubss xmm0, xmm0, dword ptr [eax+8]
        vmovss xmm1, dword ptr [esi+1Ch]
        vsubss xmm1, xmm1, dword ptr [eax+4]
        vmovss xmm2, dword ptr [esi+40h]
        vsubss xmm2, xmm2, dword ptr [eax]
        vmulss xmm1, xmm1, xmm1
        vfmadd231ss xmm1, xmm2, xmm2
        vfmadd231ss xmm1, xmm0, xmm0
        vcvtss2sd xmm0, xmm1, xmm1
        vsqrtsd xmm0, xmm0, xmm0
        vcvtsd2ss xmm0, xmm0, xmm0
        vucomiss xmm0, dword_103F4BE0
      }
      if ( !(v89 | v90) ) /*0x1002f356*/
        goto LABEL_61; /*0x1002f356*/
      if ( byte_103F4D19 == 1 ) /*0x1002f363*/
      {
        __asm /*0x1002f369*/
        {
          vcvttss2si edx, dword ptr [esi+0D8h]
          vcvttss2si ebx, dword ptr [esi+54h]
          vcvttss2si eax, dword ptr [esi+3Ch]
        }
        if ( dword_103F4D1C == 1 ) /*0x1002f384*/
        {
          v1858 = _EDX; /*0x1002fbc9*/
          v1859 = _EBX; /*0x1002fbcf*/
          __asm { vcvttss2si edx, dword ptr [esi+0DCh] } /*0x1002fbd5*/
          v1734 = (wchar_t *)_EDX; /*0x1002fbdd*/
          v1860 = _EAX; /*0x1002fbe0*/
          __asm { vcvttss2si eax, dword ptr [esi+5Ch] } /*0x1002fbe6*/
          v1727 = _EAX; /*0x1002fbeb*/
          v1861 = -1; /*0x1002fbee*/
          __asm { vcvttss2si edi, dword ptr [esi+58h] } /*0x1002fbf5*/
          __asm { vcvttss2si edx, dword ptr [esi+34h] }
          sub_101778E0(a1: _EDX, a2: _EDI, a3: _EAX, a4: v1734, a5: v1858, a6: dword_103F4D30); /*0x1002fc15*/
        }
        else if ( dword_103F4D1C == 0 ) /*0x1002f38c*/
        {
          v127 = dword_103F4D30; /*0x1002f392*/
          v1862 = _EDX; /*0x1002f398*/
          v1863 = _EBX; /*0x1002f3a4*/
          v1864 = _EAX; /*0x1002f3aa*/
          v1865 = -1; /*0x1002f3b0*/
          __asm /*0x1002f3b7*/
          {
            vmovss xmm0, dword ptr [esi+0DCh]
            vmovss dword ptr [esi+1Ch], xmm0
            vcvttss2si eax, xmm0
          }
          v1735 = _EAX; /*0x1002f3c8*/
          __asm /*0x1002f3cb*/
          {
            vmovss xmm0, dword ptr [esi+5Ch]
            vmovss dword ptr [esi+60h], xmm0
            vcvttss2si eax, xmm0
          }
          v1746 = _EAX; /*0x1002f3d9*/
          __asm /*0x1002f3df*/
          {
            vmovss xmm1, dword ptr [esi+58h]
            vmovss dword ptr [esi+3Ch], xmm1
            vmovss xmm0, dword ptr [esi+34h]
            vmovss dword ptr [esi+40h], xmm0
            vcvttss2si ebx, xmm1
            vcvttss2si edx, xmm0
          }
          sub_10177840(a1: _EDX, a2: _EBX, a3: v1746, a4: v1735, a5: v1862, a6: v127); /*0x1002f40e*/
          __asm /*0x1002f416*/
          {
            vcvtsi2ss xmm0, xmm3, eax
            vaddss xmm1, xmm0, dword ptr [esi+1Ch]
            vcvttss2si eax, xmm1
          }
          v1735 = _EAX; /*0x1002f423*/
          __asm /*0x1002f426*/
          {
            vaddss xmm0, xmm0, dword ptr [esi+60h]
            vcvttss2si edx, xmm0
            vcvtsi2ss xmm0, xmm3, edi
          }
          __asm
          {
            vmovss xmm1, dword ptr [esi+3Ch]
            vsubss xmm1, xmm1, xmm0
            vcvttss2si edi, xmm1
          }
          v1955 = -16777216; /*0x1002f446*/
          __asm /*0x1002f450*/
          {
            vmovss xmm1, dword ptr [esi+40h]
            vsubss xmm0, xmm1, xmm0
            vcvttss2si ebx, xmm0
          }
          sub_10177840(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: 0, a6: 1); /*0x1002f46c*/
          __asm /*0x1002f471*/
          {
            vmovss xmm1, ds:dword_10185850
            vaddss xmm0, xmm1, dword ptr [esi+1Ch]
            vcvttss2si eax, xmm0
          }
          v1727 = _EAX; /*0x1002f482*/
          __asm /*0x1002f485*/
          {
            vaddss xmm0, xmm1, dword ptr [esi+60h]
            vcvttss2si edx, xmm0
          }
          __asm
          {
            vmovss xmm1, ds:dword_10184184
            vaddss xmm0, xmm1, dword ptr [esi+3Ch]
            vcvttss2si edi, xmm0
          }
          v1954 = -16777216; /*0x1002f4a5*/
          __asm /*0x1002f4af*/
          {
            vaddss xmm0, xmm1, dword ptr [esi+40h]
            vcvttss2si ebx, xmm0
          }
          sub_10177840(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: 0, a6: 1); /*0x1002f4c7*/
        }
      }
      if ( byte_103F4D34 == 1 ) /*0x1002fc23*/
      {
        __asm { vcvttss2si eax, dword ptr [esi+30h] } /*0x1002fc29*/
        v1734 = (wchar_t *)_EAX; /*0x1002fc2e*/
        __asm { vcvttss2si eax, dword ptr [esi+24h] } /*0x1002fc31*/
        v1727 = _EAX; /*0x1002fc36*/
        __asm { vcvttss2si eax, dword ptr [esi+14h] } /*0x1002fc39*/
        v1725 = _EAX; /*0x1002fc3e*/
        if ( dword_103F4D38 == 1 ) /*0x1002fc49*/
        {
          __asm /*0x1002fddd*/
          {
            vxorps xmm0, xmm0, xmm0
            vmovups ymmword ptr [esi+230h], ymm0
            vmovaps ymmword ptr [esi+220h], ymm0
            vmovaps ymmword ptr [esi+200h], ymm0
            vmovaps ymmword ptr [esi+1E0h], ymm0
            vmovaps ymmword ptr [esi+1C0h], ymm0
          }
          v265 = dword_103EC770; /*0x1002fe09*/
          __asm { vzeroupper } /*0x1002fe14*/
          v266 = (*(int (__thiscall **)(wchar_t *))(*(_DWORD *)v1728 + 32))(a1: v1728); /*0x1002fe17*/
          if ( (*(unsigned __int8 (__thiscall **)(int, int, float **))(*(_DWORD *)v265 + 32))( /*0x1002fe2d*/
                 a1: v265,
                 a2: v266,
                 a3: &v1897) != 0 )
          {
            __asm /*0x1002fe33*/
            {
              vmovss xmm0, dword ptr [esi+34h]
              vaddss xmm0, xmm0, dword ptr [esi+5Ch]
              vaddss xmm0, xmm0, ds:dword_10185854
              vmovss dword ptr [esi+20h], xmm0
              vxorps xmm0, xmm0, xmm0
              vaddss xmm0, xmm0, dword ptr [esi+58h]
              vmovss dword ptr [esi+24h], xmm0
            }
            sub_1017B960(a1: v1744); /*0x1002fe65*/
            v1738 = *(wchar_t **)ArgList; /*0x1002fe6f*/
            sub_1000E5E0(a1: v1741, Str: (char *)v1899); /*0x1002fe81*/
            v2004 = 9; /*0x1002fe86*/
            sub_1002A600(lpWideCharStr: (LPWSTR)v1744, lpMultiByteStr: (LPCCH)v1741); /*0x1002fe94*/
            v1850 = (char)v1734; /*0x1002fe9c*/
            v1734 = (wchar_t *)v1744[0]; /*0x1002fea8*/
            v1851 = v1727; /*0x1002feae*/
            v1727 = v1745; /*0x1002feba*/
            v1852 = (char)v1725; /*0x1002fec0*/
            v1853 = -1; /*0x1002fec6*/
            v2004 = 10; /*0x1002fed3*/
            sub_1017B960(a1: &v1739); /*0x1002fee1*/
            __asm { vcvttss2si eax, dword ptr [esi+20h] } /*0x1002fee6*/
            __asm { vcvttss2si ecx, dword ptr [esi+24h] }
            v274 = v1734; /*0x1002fefa*/
            if ( v1727 < 8 ) /*0x1002fefd*/
              v274 = (wchar_t *)v1744; /*0x1002fefd*/
            sub_10177410( /*0x1002ff13*/
              a1: (int)v1738,
              a2: (int)&v1739,
              a3: _EAX,
              a4: _ECX,
              a5: v1850,
              a6: 1,
              Format: v274,
              ArgList: v1724[0]);
            sub_1000BCE0(a1: v1744); /*0x1002ff21*/
            sub_1000BC90(a1: v1741); /*0x1002ff2c*/
          }
        }
        else if ( dword_103F4D38 == 0 ) /*0x1002fc51*/
        {
          __asm /*0x1002fc57*/
          {
            vxorps xmm0, xmm0, xmm0
            vmovups ymmword ptr [esi+230h], ymm0
            vmovaps ymmword ptr [esi+220h], ymm0
            vmovaps ymmword ptr [esi+200h], ymm0
            vmovaps ymmword ptr [esi+1E0h], ymm0
            vmovaps ymmword ptr [esi+1C0h], ymm0
          }
          v250 = dword_103EC770; /*0x1002fc83*/
          __asm { vzeroupper } /*0x1002fc8e*/
          v251 = (*(int (__thiscall **)(wchar_t *))(*(_DWORD *)v1728 + 32))(a1: v1728); /*0x1002fc91*/
          if ( (*(unsigned __int8 (__thiscall **)(int, int, float **))(*(_DWORD *)v250 + 32))( /*0x1002fca5*/
                 a1: v250,
                 a2: v251,
                 a3: &v1897) != 0 )
          {
            v1729 = *(_DWORD *)ArgList; /*0x1002fcb0*/
            sub_1000E5E0(a1: v1741, Str: (char *)v1899); /*0x1002fcc2*/
            v2004 = 7; /*0x1002fcc7*/
            sub_1002A600(lpWideCharStr: (LPWSTR)v1744, lpMultiByteStr: (LPCCH)v1741); /*0x1002fcd9*/
            v1738 = (wchar_t *)v1744; /*0x1002fceb*/
            if ( v1745 >= 8 ) /*0x1002fcee*/
              v1738 = (wchar_t *)v1744[0]; /*0x1002fcf6*/
            v1854 = (char)v1734; /*0x1002fcfc*/
            v1855 = v1727; /*0x1002fd05*/
            v1856 = (char)v1725; /*0x1002fd0e*/
            v1857 = -1; /*0x1002fd14*/
            __asm /*0x1002fd1b*/
            {
              vmovss xmm0, dword ptr [esi+58h]
              vmovss dword ptr [esi+14h], xmm0
            }
            v2004 = 8; /*0x1002fd2b*/
            sub_1017B960(a1: &v1739); /*0x1002fd39*/
            v1734 = (wchar_t *)v1740; /*0x1002fd44*/
            __asm /*0x1002fd47*/
            {
              vmovss xmm0, dword ptr [esi+34h]
              vmovss dword ptr [esi+1Ch], xmm0
              vmovss xmm0, dword ptr [esi+5Ch]
              vmovss dword ptr [esi+20h], xmm0
            }
            sub_1017B960(a1: v1742); /*0x1002fd68*/
            __asm /*0x1002fd6d*/
            {
              vmovss xmm0, dword ptr [esi+20h]
              vmulss xmm0, xmm0, ds:dword_10184EDC
              vaddss xmm0, xmm0, dword ptr [esi+1Ch]
              vcvttss2si eax, xmm0
            }
            __asm
            {
              vcvtsi2ss xmm0, xmm3, ecx
              vmovss xmm1, dword ptr [esi+14h]
              vsubss xmm0, xmm1, xmm0
              vaddss xmm0, xmm0, ds:dword_10185850
              vcvttss2si ecx, xmm0
            }
            sub_10177410( /*0x1002fdb8*/
              a1: v1729,
              a2: (int)v1742,
              a3: _EAX,
              a4: _ECX,
              a5: v1854,
              a6: 8,
              Format: v1738,
              ArgList: v1724[0]);
            sub_1000BCE0(a1: v1744); /*0x1002fdc6*/
            sub_1000BC90(a1: v1741); /*0x1002fdd1*/
          }
        }
      }
      v275 = v1730; /*0x1002ff36*/
      if ( byte_103F4D64 == 1 ) /*0x1002ff40*/
      {
        if ( dword_103F4D68 == 1 ) /*0x1002ff4e*/
        {
          v1725 = *(char **)ArgList; /*0x10030037*/
          v287 = *(int (__thiscall **)(char *))(*(_DWORD *)v1730 + 468); /*0x1003003c*/
          v2004 = -1; /*0x10030042*/
          v1734 = (wchar_t *)v287(a1: v1730); /*0x10030051*/
          __asm /*0x1003005a*/
          {
            vmovss xmm0, dword ptr [esi+58h]
            vaddss xmm0, xmm0, dword ptr [esi+0DCh]
            vmovss xmm1, ds:dword_10185854
            vaddss xmm0, xmm0, xmm1
            vaddss xmm0, xmm0, xmm1
            vaddss xmm0, xmm0, xmm1
            vcvttss2si ebx, xmm0
            vmovss xmm0, ds:dword_10184EDC
            vmulss xmm0, xmm0, dword ptr [esi+5Ch]
            vaddss xmm0, xmm0, dword ptr [esi+34h]
          }
          v1952 = v1896; /*0x10030091*/
          __asm { vcvttss2si edi, xmm0 } /*0x10030097*/
          sub_1017B960(a1: &v1897); /*0x100300a8*/
          sub_10177250( /*0x100300ca*/
            a1: (int)v1725,
            a2: (int)&v1897,
            a3: _EDI,
            a4: _EBX,
            a5: v1952,
            a6: 8,
            Format: (char *)"%d",
            ArgList: (char)v1734);
        }
        else
        {
          if ( dword_103F4D68 != 0 ) /*0x1002ff56*/
            goto LABEL_130; /*0x1002ff56*/
          v1725 = *(char **)ArgList; /*0x1002ff61*/
          v276 = *(int (__thiscall **)(char *))(*(_DWORD *)v1730 + 468); /*0x1002ff66*/
          v2004 = -1; /*0x1002ff6c*/
          v1734 = (wchar_t *)v276(a1: v1730); /*0x1002ff7b*/
          v1953 = v1896; /*0x1002ff84*/
          __asm /*0x1002ff8a*/
          {
            vmovss xmm0, dword ptr [esi+58h]
            vmovss dword ptr [esi+1Ch], xmm0
          }
          sub_1017B960(a1: &v1897); /*0x1002ffa1*/
          __asm /*0x1002ffad*/
          {
            vcvtsi2ss xmm0, xmm3, ebx
            vaddss xmm0, xmm0, dword ptr [esi+1Ch]
            vcvttss2si ebx, xmm0
            vmovss xmm0, dword ptr [esi+34h]
            vaddss xmm0, xmm0, dword ptr [esi+5Ch]
            vaddss xmm0, xmm0, ds:dword_10185854
            vcvttss2si edi, xmm0
          }
          sub_1017B960(a1: v1744); /*0x1002ffdd*/
          sub_10177250( /*0x1002fffb*/
            a1: (int)v1725,
            a2: (int)v1744,
            a3: _EDI,
            a4: _EBX,
            a5: v1953,
            a6: 1,
            Format: (char *)"%d",
            ArgList: (char)v1734);
        }
        v275 = v1730; /*0x100300cf*/
      }
LABEL_130:
      if ( byte_103F4D84 == 1 ) /*0x100300dc*/
      {
        if ( dword_103F4D88 == 1 ) /*0x100300ea*/
        {
          v309 = *(int (__thiscall **)(char *))(*(_DWORD *)v275 + 468); /*0x10030178*/
          v2004 = -1; /*0x1003017e*/
          v310 = v309(a1: v1730); /*0x1003018d*/
          v311 = (v310 != 0) & (*(_DWORD *)sub_10035E70(this: v1730) != 0); /*0x100301a3*/
          v275 = v1730; /*0x100301a5*/
          if ( v311 == 1 ) /*0x100301ab*/
          {
            __asm /*0x100301b1*/
            {
              vcvtsi2ss xmm2, xmm3, edi
              vcvtsi2ss xmm0, xmm3, eax
              vmaxss xmm3, xmm2, xmm0
              vmovss xmm0, dword ptr [esi+58h]
              vaddss xmm0, xmm0, dword ptr [esi+0DCh]
              vaddss xmm0, xmm0, ds:dword_10185854
              vmovss xmm1, dword ptr [esi+5Ch]
              vcvttps2dq xmm4, xmm1
              vcvtdq2ps xmm4, xmm4
              vucomiss xmm3, ds:dword_10184F30
            }
            if ( __SETP__(1, 1) ) /*0x100301ed*/
            {
              __asm /*0x10030962*/
              {
                vdivss xmm2, xmm2, xmm3
                vxorps xmm6, xmm6, xmm6
                vmaxss xmm3, xmm6, xmm2
                vmovss xmm5, ds:dword_10184184
                vcmpltss xmm2, xmm5, xmm2
                vbroadcastss xmm5, ds:dword_10184184
                vblendvps xmm2, xmm3, xmm5, xmm2
                vfmadd213ss xmm2, xmm4, xmm6
              }
            }
            else
            {
              __asm /*0x100301f3*/
              {
                vcmpless xmm2, xmm3, xmm2
                vandps xmm2, xmm2, xmm4
              }
            }
            __asm /*0x1003098f*/
            {
              vcvttss2si ebx, xmm0
              vcvttss2si eax, xmm1
              vcvttss2si ecx, xmm2
            }
            v1725 = _ECX; /*0x1003099b*/
            v1993 = -16119286; /*0x100309a4*/
            __asm /*0x100309b1*/
            {
              vmovss xmm0, dword ptr [esi+34h]
              vmovss dword ptr [esi+40h], xmm0
              vaddss xmm0, xmm0, ds:dword_1018555C
            }
            __asm { vcvttss2si edi, xmm0 }
            sub_101777E0(a1: _EDI, a2: _EBX - 1, a3: _EAX + 2, a4: 4, a5: -16119286); /*0x100309d4*/
            v1992 = v1896; /*0x100309e5*/
            __asm { vcvttss2si edx, dword ptr [esi+40h] } /*0x100309eb*/
            *(_DWORD *)&v1723[16] = v1896; /*0x100309f0*/
            *(_DWORD *)&v1723[12] = 2; /*0x100309f1*/
            *(_DWORD *)&v1723[8] = v1725; /*0x100309f3*/
            *(_DWORD *)&v1723[4] = _EBX; /*0x100309f6*/
            *(_DWORD *)v1723 = _EDX; /*0x100309f7*/
            goto LABEL_182; /*0x100309f7*/
          }
        }
        else if ( dword_103F4D88 == 0 ) /*0x100300f2*/
        {
          v299 = *(int (__thiscall **)(char *))(*(_DWORD *)v275 + 468); /*0x100300fa*/
          v2004 = -1; /*0x10030100*/
          v300 = v299(a1: v1730); /*0x1003010f*/
          v301 = (v300 != 0) & (*(_DWORD *)sub_10035E70(this: v1730) != 0); /*0x10030125*/
          v275 = v1730; /*0x10030127*/
          if ( v301 == 1 ) /*0x1003012d*/
          {
            __asm /*0x10030133*/
            {
              vcvtsi2ss xmm2, xmm3, edi
              vcvtsi2ss xmm0, xmm3, eax
              vmovss xmm1, ds:dword_10185858
              vaddss xmm1, xmm1, dword ptr [esi+34h]
              vmaxss xmm3, xmm2, xmm0
              vmovss xmm0, dword ptr [esi+0DCh]
              vucomiss xmm3, ds:dword_10184F30
            }
            if ( __SETP__(1, 1) ) /*0x10030162*/
            {
              __asm /*0x1003062a*/
              {
                vdivss xmm2, xmm2, xmm3
                vxorps xmm5, xmm5, xmm5
                vmaxss xmm3, xmm5, xmm2
                vmovss xmm4, ds:dword_10184184
                vcmpltss xmm2, xmm4, xmm2
                vbroadcastss xmm4, ds:dword_10184184
                vblendvps xmm2, xmm3, xmm4, xmm2
                vfmadd213ss xmm2, xmm0, xmm5
              }
            }
            else
            {
              __asm /*0x10030168*/
              {
                vcmpless xmm2, xmm3, xmm2
                vandps xmm2, xmm2, xmm0
              }
            }
            __asm { vcvttss2si eax, xmm1 } /*0x10030657*/
            v1734 = (wchar_t *)_EAX; /*0x1003065b*/
            __asm { vcvttss2si edx, xmm2 } /*0x1003065e*/
            v1725 = _EDX; /*0x10030662*/
            v1995 = -16119286; /*0x1003066b*/
            __asm { vaddss xmm0, xmm0, dword ptr [esi+58h] } /*0x10030675*/
            v405 = _EDX + 2; /*0x1003067a*/
            __asm /*0x1003067d*/
            {
              vcvttps2dq xmm1, xmm2
              vcvtdq2ps xmm1, xmm1
              vsubss xmm0, xmm0, xmm1
              vmovss dword ptr [esi+1Ch], xmm0
              vaddss xmm0, xmm0, ds:dword_1018555C
              vcvttss2si edx, xmm0
            }
            sub_101777E0(a1: _EAX - 1, a2: _EDX, a3: 4, a4: v405, a5: -16119286); /*0x100306a7*/
            v1994 = v1896; /*0x100306b8*/
            __asm { vcvttss2si edx, dword ptr [esi+1Ch] } /*0x100306be*/
            *(_DWORD *)&v1723[16] = v1896; /*0x100306c3*/
            *(_DWORD *)&v1723[12] = v1725; /*0x100306c4*/
            *(_DWORD *)&v1723[8] = 2; /*0x100306c7*/
            *(_DWORD *)&v1723[4] = _EDX; /*0x100306c9*/
            *(_DWORD *)v1723 = v1734; /*0x100306ca*/
LABEL_182:
            sub_10177780( /*0x100309f8*/
              a1: *(_DWORD *)v1723,
              a2: *(_DWORD *)&v1723[4],
              a3: *(_DWORD *)&v1723[8],
              a4: *(_DWORD *)&v1723[12],
              a5: *(_DWORD *)&v1723[16]);
            v275 = v1730; /*0x100309fd*/
          }
        }
      }
      if ( v275 != v1748 && byte_103F4DA4 != 0 ) /*0x10030a13*/
      {
        __asm /*0x10030a19*/
        {
          vcvttss2si eax, dword ptr [esi+104h]
          vcvttss2si edx, dword ptr [esi+0ECh]
          vcvttss2si ebx, dword ptr [esi+100h]
        }
        LOBYTE(v1849) = _EAX; /*0x10030a37*/
        BYTE1(v1849) = _EDX; /*0x10030a3d*/
        BYTE2(v1849) = _EBX; /*0x10030a43*/
        HIBYTE(v1849) = -1; /*0x10030a49*/
        __asm { vmovss xmm0, dword ptr [esi+58h] } /*0x10030a55*/
        if ( dword_103F4DA8 != 2 ) /*0x10030a5d*/
        {
          if ( dword_103F4DA8 == 1 ) /*0x10030a62*/
          {
            __asm /*0x10030a64*/
            {
              vaddss xmm0, xmm0, dword ptr [esi+0DCh]
              vmovss xmm1, ds:dword_10184EDC
              vmulss xmm1, xmm1, dword ptr [esi+5Ch]
              vaddss xmm1, xmm1, dword ptr [esi+34h]
            }
            goto LABEL_190; /*0x10030a7e*/
          }
          __asm /*0x10030aac*/
          {
            vmovss xmm1, ds:dword_10184EDC
            vmulss xmm1, xmm1, dword ptr [esi+5Ch]
            vaddss xmm1, xmm1, dword ptr [esi+34h]
          }
          if ( dword_103F4DA8 != 0 ) /*0x10030ac0*/
          {
LABEL_190:
            v469 = *(_DWORD *)(*(_DWORD *)ArgList + 4); /*0x10030ac6*/
LABEL_191:
            v2004 = -1; /*0x10030ac9*/
          }
          else
          {
            v469 = 0; /*0x10030c0e*/
          }
          __asm { vcvttss2si eax, xmm0 } /*0x10030ad3*/
          v1725 = _EAX; /*0x10030ad7*/
          __asm { vcvttss2si ebx, xmm1 } /*0x10030ae3*/
          v474 = **(_DWORD **)ArgList / 2; /*0x10030ae7*/
          v2004 = -1; /*0x10030ae9*/
          *(_DWORD *)&v1723[12] = _EAX; /*0x10030afa*/
          *(_DWORD *)&v1723[8] = _EBX; /*0x10030afd*/
          *(_DWORD *)&v1723[4] = v469; /*0x10030afe*/
          *(_DWORD *)v1723 = v474; /*0x10030aff*/
          sub_101775B0(a1: *(__int128 *)v1723, a2: v1849); /*0x10030b00*/
          v275 = v1730; /*0x10030b05*/
          goto LABEL_193; /*0x10030b05*/
        }
        __asm /*0x10030a80*/
        {
          vmovss xmm2, ds:dword_10184EDC
          vmulss xmm1, xmm2, dword ptr [esi+0DCh]
          vaddss xmm0, xmm0, xmm1
          vmulss xmm1, xmm2, dword ptr [esi+5Ch]
          vaddss xmm1, xmm1, dword ptr [esi+34h]
        }
        v469 = *(_DWORD *)(*(_DWORD *)ArgList + 4) / 2; /*0x10030aa8*/
        goto LABEL_191; /*0x10030aaa*/
      }
LABEL_193:
      v38 = v1743; /*0x10030b08*/
      if ( byte_103F4DBC == 1 ) /*0x10030b15*/
      {
        __asm { vcvttss2si eax, dword ptr [esi+260h] } /*0x10030b17*/
        v476 = v275; /*0x10030b1f*/
        __asm /*0x10030b21*/
        {
          vcvttss2si ecx, dword ptr [esi+0F8h]
          vcvttss2si edx, dword ptr [esi+0F0h]
        }
        v1845 = _EAX; /*0x10030b31*/
        v1846 = _ECX; /*0x10030b37*/
        v1847 = _EDX; /*0x10030b3d*/
        v1848 = -1; /*0x10030b43*/
        v2004 = -1; /*0x10030b4a*/
        sub_1002E810(a1: v476, a2: _EAX); /*0x10030b5c*/
      }
      if ( byte_103F4DD0 == 1 ) /*0x10030b68*/
      {
        __asm /*0x10030b6e*/
        {
          vcvttss2si eax, dword ptr [esi+0F4h]
          vcvttss2si edx, dword ptr [esi+0E4h]
          vcvttss2si ebx, dword ptr [esi+0E8h]
          vmovss xmm0, dword ptr [esi+34h]
          vmovss xmm1, ds:dword_10184EDC
          vmulss xmm1, xmm1, dword ptr [esi+5Ch]
          vmovss xmm2, ds:dword_10185850
          vaddss xmm2, xmm2, dword ptr [esi+58h]
          vbroadcastss xmm2, xmm2
          vblendps xmm1, xmm1, xmm2, 2
          vblendps xmm0, xmm0, ds:xmmword_10185860, 0Eh
          vaddps xmm0, xmm0, xmm1
          vmovddup xmm1, xmm0
          vaddps xmm1, xmm1, ds:xmmword_10185870
          vmovups xmmword ptr [esi+1C0h], xmm1
          vmovlps qword ptr [esi+1D0h], xmm0
        }
        LOBYTE(v1844) = _EAX; /*0x10030be0*/
        BYTE1(v1844) = _EDX; /*0x10030be6*/
        BYTE2(v1844) = _EBX; /*0x10030bec*/
        HIBYTE(v1844) = -1; /*0x10030bf2*/
        v2004 = -1; /*0x10030bf9*/
        v447 = v1844; /*0x10030c03*/
        goto LABEL_60; /*0x10030c09*/
      }
LABEL_61:
      ++v38; /*0x1002edb0*/
    }
  }
  if ( (v36 & 1) != 0 ) /*0x10030d2f*/
  {
    v493 = *(void (__thiscall **)(int, int))(*(_DWORD *)dword_103EC728 + 420); /*0x10030d3d*/
    v2004 = -1; /*0x10030d43*/
    v493(a1: dword_103EC728, a2: 1065353216); /*0x10030d52*/
    if ( (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC7B8 + 32))(a1: dword_103EC7B8) > 0 ) /*0x10030d61*/
    {
      v494 = nullptr; /*0x10030d67*/
      do /*0x10030dcd*/
      {
        v500 = *(_DWORD *)dword_103EC7B8; /*0x10030dcd*/
        v1730 = v494 + 1; /*0x10030dcf*/
        v501 = (*(int (__thiscall **)(int, char *))(v500 + 12))(a1: dword_103EC7B8, a2: v494 + 1); /*0x10030dd3*/
        if ( v501 == 0 ) /*0x10030dd8*/
          goto LABEL_209; /*0x10030dd8*/
        v502 = v501; /*0x10030dda*/
        v503 = v501 + 8; /*0x10030dde*/
        if ( (*(unsigned __int8 (__thiscall **)(int))(*(_DWORD *)(v501 + 8) + 28))(a1: v501 + 8) != 0 ) /*0x10030deb*/
          goto LABEL_209; /*0x10030deb*/
        v504 = (*(int (__thiscall **)(int))(*(_DWORD *)v503 + 4))(a1: v503); /*0x10030df1*/
        if ( v504 == 0 ) /*0x10030df6*/
          goto LABEL_209; /*0x10030df6*/
        v505 = v504; /*0x10030df8*/
        _EAX = (*(int (__thiscall **)(int))(*(_DWORD *)v502 + 44))(a1: v502); /*0x10030dfe*/
        __asm /*0x10030e01*/
        {
          vmovss xmm0, dword ptr [eax]
          vmovss xmm1, dword ptr [eax+4]
          vmovss xmm2, dword ptr [eax+8]
        }
        v1741[0] = 0.0; /*0x10030e0f*/
        v1739 = 0; /*0x10030e19*/
        v1742[0] = 0; /*0x10030e20*/
        v1732[0] = 0; /*0x10030e2a*/
        v510 = *(_DWORD *)(v505 + 20); /*0x10030e31*/
        __asm /*0x10030e39*/
        {
          vmovss dword ptr [esi+18h], xmm0
          vmovss dword ptr [esi+14h], xmm1
          vmovss dword ptr [esi+40h], xmm2
        }
        if ( v510 > 263 ) /*0x10030e48*/
        {
          if ( v510 != 264 ) /*0x10030eb5*/
          {
            if ( v510 != 277 ) /*0x10030ec0*/
              goto LABEL_209; /*0x10030ec0*/
            if ( *(_WORD *)sub_1002A670(a1: v502) == 0 ) /*0x10030ed1*/
              goto LABEL_209; /*0x10030ed1*/
            v1727 = (unsigned int)off_1029E57C; /*0x10030edc*/
            v1738 = *(wchar_t **)(v505 + 20); /*0x10030ee2*/
            v513 = *(_DWORD *)sub_1002A740(a1: v502); /*0x10030eec*/
            v514 = (_WORD *)sub_1002A670(a1: v502); /*0x10030ef0*/
            if ( (unsigned __int8)sub_101669A0(a1: *v514, a2: v513, a3: v1738) == 0 ) /*0x10030f07*/
              goto LABEL_209; /*0x10030f07*/
            if ( (unsigned __int8)sub_1002E000(a1: v502, a2: v1741, a3: &v1739, a4: v1742, a5: v1732) == 0 ) /*0x10030f2b*/
              goto LABEL_209; /*0x10030f2b*/
            (*(void (__thiscall **)(char *))(*(_DWORD *)v1748 + 44))(a1: v1748); /*0x10030f39*/
            __asm /*0x10030f3c*/
            {
              vmovss xmm0, dword ptr [esi+40h]
              vsubss xmm0, xmm0, dword ptr [eax+8]
              vmovss xmm1, dword ptr [esi+14h]
              vsubss xmm1, xmm1, dword ptr [eax+4]
              vmovss xmm2, dword ptr [esi+18h]
              vsubss xmm2, xmm2, dword ptr [eax]
              vmulss xmm1, xmm1, xmm1
              vfmadd231ss xmm1, xmm2, xmm2
              vfmadd231ss xmm1, xmm0, xmm0
              vcvtss2sd xmm0, xmm1, xmm1
              vsqrtsd xmm0, xmm0, xmm0
              vcvtsd2ss xmm0, xmm0, xmm0
              vucomiss xmm0, dword_103F4F00
            }
            if ( !(v89 | v90) || byte_103F4ED0 != 1 || byte_103F4ED1 != 1 ) /*0x10030f95*/
              goto LABEL_209; /*0x10030f95*/
            __asm /*0x10030f9b*/
            {
              vmovss xmm1, ds:dword_10184380
              vmulss xmm0, xmm1, dword ptr xmmword_103F4F04
              vcvttss2si eax, xmm0
            }
            i = _EAX; /*0x10030faf*/
            __asm /*0x10030fb2*/
            {
              vmulss xmm0, xmm1, dword ptr xmmword_103F4F04+4
              vcvttss2si eax, xmm0
            }
            v1725 = _EAX; /*0x10030fbe*/
            __asm /*0x10030fc1*/
            {
              vmulss xmm0, xmm1, dword ptr xmmword_103F4F04+8
              vcvttss2si eax, xmm0
            }
            v1734 = (wchar_t *)_EAX; /*0x10030fcd*/
            if ( byte_103F4ED4 == 1 ) /*0x10030fd7*/
            {
              v1840 = (char)i; /*0x10030fdc*/
              v1841 = (char)v1725; /*0x10030fe5*/
              v1842 = (char)v1734; /*0x10030fee*/
              v1843 = -1; /*0x10030ff4*/
              sub_1002E810(a1: v502, a2: (char)i); /*0x10031003*/
            }
            if ( byte_103F4ED2 == 1 ) /*0x1003100f*/
            {
              if ( dword_103F4EE0 == 1 ) /*0x1003101d*/
              {
                v1835 = (char)i; /*0x10031fd1*/
                __asm { vcvttss2si eax, dword ptr [esi+34h] } /*0x10031fd7*/
                v1727 = _EAX; /*0x10031fdc*/
                v1836 = (char)v1725; /*0x10031fe2*/
                __asm { vcvttss2si edx, dword ptr [esi+0A8h] } /*0x10031fe8*/
                v1837 = (char)v1734; /*0x10031ff3*/
                __asm { vcvttss2si edi, dword ptr [esi+7Ch] } /*0x10031ff9*/
                v1838 = -1; /*0x10031ffe*/
                __asm { vcvttss2si ebx, dword ptr [esi+90h] } /*0x10032005*/
                sub_101778E0(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: (char)i, a6: 1); /*0x1003201c*/
              }
              else if ( dword_103F4EE0 == 0 ) /*0x10031025*/
              {
                LOBYTE(v1839) = (_BYTE)i; /*0x10031034*/
                __asm /*0x1003103a*/
                {
                  vmovss xmm0, dword ptr [esi+34h]
                  vmovss dword ptr [esi+1Ch], xmm0
                  vcvttss2si eax, xmm0
                }
                v1731 = _EAX; /*0x10031048*/
                BYTE1(v1839) = (_BYTE)v1725; /*0x1003104e*/
                __asm /*0x10031054*/
                {
                  vmovss xmm0, dword ptr [esi+0A8h]
                  vmovss dword ptr [esi+60h], xmm0
                  vcvttss2si edx, xmm0
                }
                BYTE2(v1839) = (_BYTE)v1734; /*0x10031068*/
                __asm /*0x1003106e*/
                {
                  vmovss xmm0, dword ptr [esi+7Ch]
                  vmovss dword ptr [esi+24h], xmm0
                  vcvttss2si edi, xmm0
                }
                HIBYTE(v1839) = -1; /*0x1003107c*/
                __asm /*0x10031083*/
                {
                  vmovss xmm0, dword ptr [esi+90h]
                  vmovss dword ptr [esi+20h], xmm0
                  vcvttss2si ebx, xmm0
                }
                sub_101777E0(a1: _EBX, a2: _EDI, a3: _EDX, a4: v1731, a5: v1839); /*0x100310a1*/
                __asm /*0x100310a6*/
                {
                  vmovss xmm1, ds:dword_10185854
                  vaddss xmm0, xmm1, dword ptr [esi+1Ch]
                  vcvttss2si eax, xmm0
                }
                v1731 = _EAX; /*0x100310b7*/
                __asm /*0x100310ba*/
                {
                  vaddss xmm0, xmm1, dword ptr [esi+60h]
                  vcvttss2si edx, xmm0
                }
                __asm
                {
                  vmovss xmm1, ds:dword_1018555C
                  vaddss xmm0, xmm1, dword ptr [esi+24h]
                  vcvttss2si edi, xmm0
                }
                v1951 = -16777216; /*0x100310da*/
                __asm /*0x100310e4*/
                {
                  vaddss xmm0, xmm1, dword ptr [esi+20h]
                  vcvttss2si ebx, xmm0
                }
                sub_101777E0(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: -16777216); /*0x100310fa*/
                __asm /*0x100310ff*/
                {
                  vmovss xmm1, ds:dword_10185850
                  vaddss xmm0, xmm1, dword ptr [esi+1Ch]
                  vcvttss2si eax, xmm0
                }
                v1727 = _EAX; /*0x10031110*/
                __asm /*0x10031113*/
                {
                  vaddss xmm0, xmm1, dword ptr [esi+60h]
                  vcvttss2si edx, xmm0
                }
                __asm
                {
                  vmovss xmm1, ds:dword_10184184
                  vaddss xmm0, xmm1, dword ptr [esi+24h]
                  vcvttss2si edi, xmm0
                }
                v1950 = -16777216; /*0x10031133*/
                __asm /*0x1003113d*/
                {
                  vaddss xmm0, xmm1, dword ptr [esi+20h]
                  vcvttss2si ebx, xmm0
                }
                sub_101777E0(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: -16777216); /*0x10031153*/
              }
            }
            if ( dword_103F4ED8 == 0 ) /*0x10032028*/
            {
              i = *(char ***)ArgList; /*0x1003213a*/
              v805 = sub_10168030(a1: off_1029E57C); /*0x10032148*/
              v806 = dword_103F4EDC; /*0x1003214a*/
              __asm /*0x10032150*/
              {
                vmovss xmm0, dword ptr [esi+7Ch]
                vmovss dword ptr [esi+14h], xmm0
              }
              sub_1017B960(a1: &v1897); /*0x10032167*/
              __asm /*0x10032173*/
              {
                vcvtsi2ss xmm0, xmm3, eax
                vmovss xmm1, dword ptr [esi+14h]
                vsubss xmm0, xmm1, xmm0
                vaddss xmm0, xmm0, ds:dword_10185850
                vaddss xmm0, xmm0, ds:dword_10185880
                vcvttss2si eax, xmm0
                vmovss xmm0, ds:dword_10184EDC
                vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                vaddss xmm0, xmm0, dword ptr [esi+90h]
                vcvttss2si edx, xmm0
              }
              sub_10178EB0(a1: _EDX, a2: _EAX, a3: v806, a4: v806, a5: v805, a6: 8); /*0x100321ba*/
              goto LABEL_209; /*0x100321bf*/
            }
            if ( dword_103F4ED8 != 1 ) /*0x10032031*/
              goto LABEL_209; /*0x10032031*/
            sub_1017B960(a1: &v1897); /*0x10032044*/
            if ( byte_103F4ED3 != 1 ) /*0x10032050*/
              goto LABEL_209; /*0x10032050*/
            v1727 = (unsigned int)v1898; /*0x1003205c*/
            v794 = dword_103F4EE4; /*0x1003205f*/
            if ( dword_103F4EE4 != 0 ) /*0x10032066*/
            {
              if ( dword_103F4EE4 == 1 ) /*0x1003206f*/
              {
LABEL_322:
                v1738 = *(wchar_t **)ArgList; /*0x10032075*/
                v1827 = (char)i; /*0x10032080*/
                __asm /*0x10032086*/
                {
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+34h]
                  vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                }
                v1828 = (char)v1725; /*0x1003209b*/
                __asm { vcvtsi2ss xmm1, xmm3, eax } /*0x100320ad*/
                v1829 = (char)v1734; /*0x100320b4*/
                __asm /*0x100320ba*/
                {
                  vsubss xmm0, xmm0, xmm1
                  vcvttss2si ebx, xmm0
                }
                v1830 = -1; /*0x100320c2*/
                __asm /*0x100320c9*/
                {
                  vmovss xmm0, dword ptr [esi+90h]
                  vaddss xmm0, xmm0, dword ptr [esi+0A8h]
                  vaddss xmm0, xmm0, ds:dword_1018531C
                  vcvttss2si edi, xmm0
                }
                sub_1017B960(a1: &v1897); /*0x100320f2*/
                sub_10177250( /*0x10032111*/
                  a1: (int)v1738,
                  a2: (int)&v1897,
                  a3: _EDI,
                  a4: _EBX,
                  a5: v1827,
                  a6: 1,
                  Format: "Witch",
                  ArgList: v1724[0]);
                v794 = dword_103F4EE4; /*0x10032119*/
                if ( dword_103F4EE4 != 2 ) /*0x10032121*/
                  goto LABEL_323; /*0x10032121*/
LABEL_347:
                v1738 = *(wchar_t **)ArgList; /*0x10032835*/
                v1823 = (char)i; /*0x10032840*/
                v1824 = (char)v1725; /*0x10032849*/
                __asm /*0x1003284f*/
                {
                  vmovss xmm0, dword ptr [esi+7Ch]
                  vaddss xmm0, xmm0, dword ptr [esi+34h]
                }
                v1825 = (char)v1734; /*0x1003285c*/
                __asm /*0x10032862*/
                {
                  vaddss xmm0, xmm0, ds:dword_10185854
                  vcvttss2si edi, xmm0
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                  vaddss xmm0, xmm0, dword ptr [esi+90h]
                }
                v1826 = -1; /*0x10032886*/
                __asm { vcvttss2si ebx, xmm0 } /*0x1003288d*/
                sub_1017B960(a1: &v1897); /*0x1003289e*/
                sub_10177250( /*0x100328bd*/
                  a1: (int)v1738,
                  a2: (int)&v1897,
                  a3: _EBX,
                  a4: _EDI,
                  a5: v1823,
                  a6: 8,
                  Format: "Witch",
                  ArgList: v1724[0]);
                if ( dword_103F4EE4 == 3 ) /*0x100328cd*/
                {
LABEL_348:
                  v1819 = (char)i; /*0x100328d3*/
                  i = *(char ***)ArgList; /*0x100328e1*/
                  v1820 = (char)v1725; /*0x100328e7*/
                  v1821 = (char)v1734; /*0x100328f0*/
                  v1822 = -1; /*0x100328f6*/
                  __asm /*0x100328fd*/
                  {
                    vmovss xmm0, ds:dword_10184EDC
                    vmulss xmm0, xmm0, dword ptr [esi+34h]
                    vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                  }
                  __asm
                  {
                    vcvtsi2ss xmm1, xmm3, eax
                    vsubss xmm0, xmm0, xmm1
                    vcvttss2si edi, xmm0
                    vmovss xmm0, ds:dword_10185884
                    vaddss xmm0, xmm0, dword ptr [esi+90h]
                    vaddss xmm0, xmm0, ds:dword_1018550C
                    vcvttss2si ebx, xmm0
                  }
                  sub_1017B960(a1: &v1897); /*0x10032950*/
                  v861 = v1819; /*0x10032955*/
                  *(_DWORD *)&v1723[16] = "Witch"; /*0x1003295b*/
                  goto LABEL_349; /*0x1003295b*/
                }
                goto LABEL_209; /*0x100328cd*/
              }
            }
            else
            {
              v1738 = *(wchar_t **)ArgList; /*0x1003276d*/
              v1831 = (char)i; /*0x10032773*/
              v1832 = (char)v1725; /*0x1003277c*/
              v1833 = (char)v1734; /*0x10032785*/
              v1834 = -1; /*0x1003278b*/
              __asm /*0x10032792*/
              {
                vmovss xmm0, dword ptr [esi+7Ch]
                vmovss dword ptr [esi+24h], xmm0
              }
              sub_1017B960(a1: &v1897); /*0x100327a9*/
              __asm /*0x100327b5*/
              {
                vcvtsi2ss xmm0, xmm3, eax
                vmovss xmm1, dword ptr [esi+24h]
                vsubss xmm0, xmm1, xmm0
                vaddss xmm0, xmm0, ds:dword_10185850
                vcvttss2si edi, xmm0
                vmovss xmm0, ds:dword_10184EDC
                vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                vaddss xmm0, xmm0, dword ptr [esi+90h]
                vcvttss2si ebx, xmm0
              }
              sub_1017B960(a1: v1744); /*0x100327f7*/
              sub_10177250( /*0x10032816*/
                a1: (int)v1738,
                a2: (int)v1744,
                a3: _EBX,
                a4: _EDI,
                a5: v1831,
                a6: 8,
                Format: "Witch",
                ArgList: v1724[0]);
              v794 = dword_103F4EE4; /*0x1003281e*/
              if ( dword_103F4EE4 == 1 ) /*0x10032826*/
                goto LABEL_322; /*0x10032826*/
            }
            if ( v794 != 2 ) /*0x1003282f*/
            {
LABEL_323:
              if ( v794 == 3 ) /*0x1003212a*/
                goto LABEL_348; /*0x1003212a*/
              goto LABEL_209; /*0x1003212a*/
            }
            goto LABEL_347; /*0x1003282f*/
          }
          if ( *(_WORD *)sub_1002A670(a1: v502) == 0 ) /*0x10031380*/
            goto LABEL_209; /*0x10031380*/
          if ( (unsigned __int8)sub_1002E000(a1: v502, a2: v1741, a3: &v1739, a4: v1742, a5: v1732) == 0 ) /*0x100313a4*/
            goto LABEL_209; /*0x100313a4*/
          v1727 = (unsigned int)off_1029E57C; /*0x100313af*/
          v1738 = *(wchar_t **)(v505 + 20); /*0x100313b5*/
          v609 = *(_DWORD *)sub_1002A740(a1: v502); /*0x100313bf*/
          v610 = (_WORD *)sub_1002A670(a1: v502); /*0x100313c3*/
          if ( (unsigned __int8)sub_101669A0(a1: *v610, a2: v609, a3: v1738) == 0 ) /*0x100313da*/
            goto LABEL_209; /*0x100313da*/
          (*(void (__thiscall **)(char *))(*(_DWORD *)v1748 + 44))(a1: v1748); /*0x100313e8*/
          __asm /*0x100313eb*/
          {
            vmovss xmm0, dword ptr [esi+40h]
            vsubss xmm0, xmm0, dword ptr [eax+8]
            vmovss xmm1, dword ptr [esi+14h]
            vsubss xmm1, xmm1, dword ptr [eax+4]
            vmovss xmm2, dword ptr [esi+18h]
            vsubss xmm2, xmm2, dword ptr [eax]
            vmulss xmm1, xmm1, xmm1
            vfmadd231ss xmm1, xmm2, xmm2
            vfmadd231ss xmm1, xmm0, xmm0
            vcvtss2sd xmm0, xmm1, xmm1
            vsqrtsd xmm0, xmm0, xmm0
            vcvtsd2ss xmm0, xmm0, xmm0
            vucomiss xmm0, dword_103F4F3C
          }
          if ( !(v89 | v90) || byte_103F4F14 != 1 || byte_103F4F15 != 1 ) /*0x10031444*/
            goto LABEL_209; /*0x10031444*/
          __asm /*0x1003144a*/
          {
            vmovss xmm1, ds:dword_10184380
            vmulss xmm0, xmm1, dword ptr xmmword_103F4F40
            vcvttss2si eax, xmm0
          }
          i = _EAX; /*0x1003145e*/
          __asm /*0x10031461*/
          {
            vmulss xmm0, xmm1, dword ptr xmmword_103F4F40+4
            vcvttss2si eax, xmm0
          }
          v1725 = _EAX; /*0x1003146d*/
          __asm /*0x10031470*/
          {
            vmulss xmm0, xmm1, dword ptr xmmword_103F4F40+8
            vcvttss2si eax, xmm0
          }
          v1734 = (wchar_t *)_EAX; /*0x1003147c*/
          if ( byte_103F4F18 == 1 ) /*0x10031486*/
          {
            v1815 = (char)i; /*0x1003148b*/
            v1816 = (char)v1725; /*0x10031494*/
            v1817 = (char)v1734; /*0x1003149d*/
            v1818 = -1; /*0x100314a3*/
            sub_1002E810(a1: v502, a2: (char)i); /*0x100314b2*/
          }
          if ( byte_103F4F16 == 1 ) /*0x100314be*/
          {
            if ( dword_103F4F1C == 1 ) /*0x100314cc*/
            {
              v1810 = (char)i; /*0x100321cd*/
              __asm { vcvttss2si eax, dword ptr [esi+34h] } /*0x100321d3*/
              v1727 = _EAX; /*0x100321d8*/
              v1811 = (char)v1725; /*0x100321de*/
              __asm { vcvttss2si edx, dword ptr [esi+0A8h] } /*0x100321e4*/
              v1812 = (char)v1734; /*0x100321ef*/
              __asm { vcvttss2si edi, dword ptr [esi+7Ch] } /*0x100321f5*/
              v1813 = -1; /*0x100321fa*/
              __asm { vcvttss2si ebx, dword ptr [esi+90h] } /*0x10032201*/
              sub_101778E0(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: (char)i, a6: 1); /*0x10032218*/
            }
            else if ( dword_103F4F1C == 0 ) /*0x100314d4*/
            {
              LOBYTE(v1814) = (_BYTE)i; /*0x100314e3*/
              __asm /*0x100314e9*/
              {
                vmovss xmm0, dword ptr [esi+34h]
                vmovss dword ptr [esi+1Ch], xmm0
                vcvttss2si eax, xmm0
              }
              v1731 = _EAX; /*0x100314f7*/
              BYTE1(v1814) = (_BYTE)v1725; /*0x100314fd*/
              __asm /*0x10031503*/
              {
                vmovss xmm0, dword ptr [esi+0A8h]
                vmovss dword ptr [esi+60h], xmm0
                vcvttss2si edx, xmm0
              }
              BYTE2(v1814) = (_BYTE)v1734; /*0x10031517*/
              __asm /*0x1003151d*/
              {
                vmovss xmm0, dword ptr [esi+7Ch]
                vmovss dword ptr [esi+24h], xmm0
                vcvttss2si edi, xmm0
              }
              HIBYTE(v1814) = -1; /*0x1003152b*/
              __asm /*0x10031532*/
              {
                vmovss xmm0, dword ptr [esi+90h]
                vmovss dword ptr [esi+20h], xmm0
                vcvttss2si ebx, xmm0
              }
              sub_101777E0(a1: _EBX, a2: _EDI, a3: _EDX, a4: v1731, a5: v1814); /*0x10031550*/
              __asm /*0x10031555*/
              {
                vmovss xmm1, ds:dword_10185854
                vaddss xmm0, xmm1, dword ptr [esi+1Ch]
                vcvttss2si eax, xmm0
              }
              v1731 = _EAX; /*0x10031566*/
              __asm /*0x10031569*/
              {
                vaddss xmm0, xmm1, dword ptr [esi+60h]
                vcvttss2si edx, xmm0
              }
              __asm
              {
                vmovss xmm1, ds:dword_1018555C
                vaddss xmm0, xmm1, dword ptr [esi+24h]
                vcvttss2si edi, xmm0
              }
              v1939 = -16777216; /*0x10031589*/
              __asm /*0x10031593*/
              {
                vaddss xmm0, xmm1, dword ptr [esi+20h]
                vcvttss2si ebx, xmm0
              }
              sub_101777E0(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: -16777216); /*0x100315a9*/
              __asm /*0x100315ae*/
              {
                vmovss xmm1, ds:dword_10185850
                vaddss xmm0, xmm1, dword ptr [esi+1Ch]
                vcvttss2si eax, xmm0
              }
              v1727 = _EAX; /*0x100315bf*/
              __asm /*0x100315c2*/
              {
                vaddss xmm0, xmm1, dword ptr [esi+60h]
                vcvttss2si edx, xmm0
              }
              __asm
              {
                vmovss xmm1, ds:dword_10184184
                vaddss xmm0, xmm1, dword ptr [esi+24h]
                vcvttss2si edi, xmm0
              }
              v1938 = -16777216; /*0x100315e2*/
              __asm /*0x100315ec*/
              {
                vaddss xmm0, xmm1, dword ptr [esi+20h]
                vcvttss2si ebx, xmm0
              }
              sub_101777E0(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: -16777216); /*0x10031602*/
            }
          }
          if ( byte_103F4F17 != 1 ) /*0x10032224*/
            goto LABEL_209; /*0x10032224*/
          sub_1017B960(a1: &v1897); /*0x10032237*/
          v1727 = (unsigned int)v1898; /*0x10032242*/
          v822 = dword_103F4F20; /*0x10032245*/
          if ( dword_103F4F20 != 0 ) /*0x1003224c*/
          {
            if ( dword_103F4F20 == 1 ) /*0x10032255*/
            {
LABEL_330:
              v1738 = *(wchar_t **)ArgList; /*0x1003225b*/
              v1802 = (char)i; /*0x10032266*/
              __asm /*0x1003226c*/
              {
                vmovss xmm0, ds:dword_10184EDC
                vmulss xmm0, xmm0, dword ptr [esi+34h]
                vaddss xmm0, xmm0, dword ptr [esi+7Ch]
              }
              v1803 = (char)v1725; /*0x10032281*/
              __asm { vcvtsi2ss xmm1, xmm3, eax } /*0x10032293*/
              v1804 = (char)v1734; /*0x1003229a*/
              __asm /*0x100322a0*/
              {
                vsubss xmm0, xmm0, xmm1
                vcvttss2si ebx, xmm0
              }
              v1805 = -1; /*0x100322a8*/
              __asm /*0x100322af*/
              {
                vmovss xmm0, dword ptr [esi+90h]
                vaddss xmm0, xmm0, dword ptr [esi+0A8h]
                vaddss xmm0, xmm0, ds:dword_1018531C
                vcvttss2si edi, xmm0
              }
              sub_1017B960(a1: &v1897); /*0x100322d8*/
              sub_10177250( /*0x100322f7*/
                a1: (int)v1738,
                a2: (int)&v1897,
                a3: _EDI,
                a4: _EBX,
                a5: v1802,
                a6: 1,
                Format: (char *)"Common",
                ArgList: v1724[0]);
              v822 = dword_103F4F20; /*0x100322ff*/
              if ( dword_103F4F20 != 2 ) /*0x10032307*/
                goto LABEL_331; /*0x10032307*/
LABEL_335:
              v1738 = *(wchar_t **)ArgList; /*0x100323e8*/
              v1798 = (char)i; /*0x100323f3*/
              v1799 = (char)v1725; /*0x100323fc*/
              __asm /*0x10032402*/
              {
                vmovss xmm0, dword ptr [esi+7Ch]
                vaddss xmm0, xmm0, dword ptr [esi+34h]
              }
              v1800 = (char)v1734; /*0x1003240f*/
              __asm /*0x10032415*/
              {
                vaddss xmm0, xmm0, ds:dword_10185854
                vcvttss2si edi, xmm0
                vmovss xmm0, ds:dword_10184EDC
                vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                vaddss xmm0, xmm0, dword ptr [esi+90h]
              }
              v1801 = -1; /*0x10032439*/
              __asm { vcvttss2si ebx, xmm0 } /*0x10032440*/
              sub_1017B960(a1: &v1897); /*0x10032451*/
              sub_10177250( /*0x10032470*/
                a1: (int)v1738,
                a2: (int)&v1897,
                a3: _EBX,
                a4: _EDI,
                a5: v1798,
                a6: 8,
                Format: (char *)"Common",
                ArgList: v1724[0]);
              if ( dword_103F4F20 == 3 ) /*0x10032480*/
              {
LABEL_336:
                v1794 = (char)i; /*0x10032486*/
                i = *(char ***)ArgList; /*0x10032494*/
                v1795 = (char)v1725; /*0x1003249a*/
                v1796 = (char)v1734; /*0x100324a3*/
                v1797 = -1; /*0x100324a9*/
                __asm /*0x100324b0*/
                {
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+34h]
                  vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                }
                __asm
                {
                  vcvtsi2ss xmm1, xmm3, eax
                  vsubss xmm0, xmm0, xmm1
                  vcvttss2si edi, xmm0
                  vmovss xmm0, ds:dword_10185884
                  vaddss xmm0, xmm0, dword ptr [esi+90h]
                  vaddss xmm0, xmm0, ds:dword_1018550C
                  vcvttss2si ebx, xmm0
                }
                sub_1017B960(a1: &v1897); /*0x10032503*/
                v861 = v1794; /*0x10032508*/
                *(_DWORD *)&v1723[16] = "Common"; /*0x1003250e*/
LABEL_349:
                sub_10177250( /*0x10032960*/
                  a1: (int)i,
                  a2: (int)&v1897,
                  a3: _EBX,
                  a4: _EDI,
                  a5: v861,
                  a6: 1,
                  Format: *(char **)&v1723[16],
                  ArgList: v1724[0]);
                goto LABEL_209; /*0x10032977*/
              }
              goto LABEL_209; /*0x10032480*/
            }
          }
          else
          {
            v1738 = *(wchar_t **)ArgList; /*0x10032320*/
            v1806 = (char)i; /*0x10032326*/
            v1807 = (char)v1725; /*0x1003232f*/
            v1808 = (char)v1734; /*0x10032338*/
            v1809 = -1; /*0x1003233e*/
            __asm /*0x10032345*/
            {
              vmovss xmm0, dword ptr [esi+7Ch]
              vmovss dword ptr [esi+24h], xmm0
            }
            sub_1017B960(a1: &v1897); /*0x1003235c*/
            __asm /*0x10032368*/
            {
              vcvtsi2ss xmm0, xmm3, eax
              vmovss xmm1, dword ptr [esi+24h]
              vsubss xmm0, xmm1, xmm0
              vaddss xmm0, xmm0, ds:dword_10185850
              vcvttss2si edi, xmm0
              vmovss xmm0, ds:dword_10184EDC
              vmulss xmm0, xmm0, dword ptr [esi+0A8h]
              vaddss xmm0, xmm0, dword ptr [esi+90h]
              vcvttss2si ebx, xmm0
            }
            sub_1017B960(a1: v1744); /*0x100323aa*/
            sub_10177250( /*0x100323c9*/
              a1: (int)v1738,
              a2: (int)v1744,
              a3: _EBX,
              a4: _EDI,
              a5: v1806,
              a6: 8,
              Format: (char *)"Common",
              ArgList: v1724[0]);
            v822 = dword_103F4F20; /*0x100323d1*/
            if ( dword_103F4F20 == 1 ) /*0x100323d9*/
              goto LABEL_330; /*0x100323d9*/
          }
          if ( v822 != 2 ) /*0x100323e2*/
          {
LABEL_331:
            if ( v822 == 3 ) /*0x10032310*/
              goto LABEL_336; /*0x10032310*/
            goto LABEL_209; /*0x10032310*/
          }
          goto LABEL_335; /*0x100323e2*/
        }
        if ( v510 != 13 ) /*0x10030e4d*/
        {
          if ( v510 == 260 ) /*0x10030e58*/
          {
            v511 = (*(int (__thiscall **)(int))(*(_DWORD *)v502 + 876))(a1: v502); /*0x10030e62*/
            v512 = v511 & ~(v511 >> 31); /*0x10030e6d*/
            if ( v512 >= 38 ) /*0x10030e7a*/
              v512 = 38; /*0x10030e7a*/
            v1727 = 4 * v512; /*0x10030e84*/
            switch ( dword_10185600[3 * v512] ) /*0x10030e97*/
            {
              case 0: /*0x10030e97*/
                if ( byte_103F4DF0 == 1 ) /*0x10030ea5*/
                  goto LABEL_262; /*0x10030ea5*/
                goto LABEL_209; /*0x10030ea5*/
              case 1: /*0x10030e97*/
                if ( byte_103F4DF1 != 0 ) /*0x10031640*/
                  goto LABEL_262; /*0x10031640*/
                goto LABEL_209; /*0x10031640*/
              case 2: /*0x10030e97*/
                if ( byte_103F4DF4 != 0 ) /*0x10031622*/
                  goto LABEL_262; /*0x10031622*/
                goto LABEL_209; /*0x10031622*/
              case 3: /*0x10030e97*/
                if ( byte_103F4DF2 != 0 ) /*0x10031631*/
                  goto LABEL_262; /*0x10031631*/
                goto LABEL_209; /*0x10031631*/
              case 4: /*0x10030e97*/
                if ( byte_103F4DF3 == 0 ) /*0x10031613*/
                  goto LABEL_209; /*0x10031613*/
LABEL_262:
                v1738 = (wchar_t *)dword_10185600[3 * v512]; /*0x10031646*/
                if ( (unsigned __int8)sub_1002E000(a1: v502, a2: v1741, a3: &v1739, a4: v1742, a5: v1732) == 0 ) /*0x10031667*/
                  goto LABEL_209; /*0x10031667*/
                (*(void (__thiscall **)(char *))(*(_DWORD *)v1748 + 44))(a1: v1748); /*0x10031675*/
                __asm /*0x1003167b*/
                {
                  vmovss xmm0, dword ptr [esi+40h]
                  vsubss xmm0, xmm0, dword ptr [eax+8]
                  vmovss xmm1, dword ptr [esi+14h]
                  vsubss xmm1, xmm1, dword ptr [eax+4]
                  vmovss xmm2, dword ptr [esi+18h]
                  vsubss xmm2, xmm2, dword ptr [eax]
                  vmulss xmm1, xmm1, xmm1
                  vfmadd231ss xmm1, xmm2, xmm2
                  vfmadd231ss xmm1, xmm0, xmm0
                  vcvtss2sd xmm0, xmm1, xmm1
                  vsqrtsd xmm0, xmm0, xmm0
                  vcvtsd2ss xmm0, xmm0, xmm0
                  vucomiss xmm0, dword_103F4DF8
                }
                if ( !(v89 | v90) || byte_103F4DE4 != 1 ) /*0x100316c7*/
                  goto LABEL_209; /*0x100316c7*/
                break; /*0x100316c7*/
              default:
                goto LABEL_209;
            }
            switch ( (unsigned int)v1738 ) /*0x100316cd*/
            {
              case 0u: /*0x100316cd*/
                *(_DWORD *)&v1723[16] = &xmmword_103F4DFC; /*0x100316d4*/
                goto LABEL_289; /*0x100316d9*/
              case 1u: /*0x100316cd*/
                *(_DWORD *)&v1723[16] = &xmmword_103F4E0C; /*0x10031b8e*/
                goto LABEL_289; /*0x10031b8e*/
              case 2u: /*0x100316cd*/
                i = (char **)sub_1002DFA0(a1: &xmmword_103F4E3C); /*0x100316f5*/
                v670 = sub_10168120(a1: v502); /*0x100316ff*/
                v671 = dword_103F4DE8; /*0x10031704*/
                if ( dword_103F4DE8 == 0 ) /*0x1003170c*/
                {
                  v750 = sub_10168520(a1: v670); /*0x10031d57*/
                  if ( v750 == 0 ) /*0x10031d5e*/
                    goto LABEL_290; /*0x10031d5e*/
                  v751 = v750; /*0x10031d64*/
                  v752 = dword_103F4E4C; /*0x10031d66*/
                  if ( dword_103F4E4C != 0 ) /*0x10031d6d*/
                  {
                    if ( dword_103F4E4C == 2 ) /*0x10031d76*/
                      goto LABEL_309; /*0x10031d76*/
LABEL_351:
                    if ( v752 == 1 ) /*0x100329e5*/
                      goto LABEL_352; /*0x100329e5*/
LABEL_310:
                    if ( v752 != 3 ) /*0x10031de2*/
                      goto LABEL_290; /*0x10031de2*/
                  }
                  else
                  {
                    v1991 = i; /*0x10032985*/
                    __asm /*0x1003298b*/
                    {
                      vmovss xmm0, ds:dword_10185888
                      vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                      vaddss xmm0, xmm0, ds:dword_10185850
                      vcvttss2si edx, xmm0
                    }
                    v1725 = _EDX; /*0x100329a4*/
                    __asm /*0x100329a7*/
                    {
                      vmovss xmm0, ds:dword_10184EDC
                      vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                      vaddss xmm0, xmm0, dword ptr [esi+90h]
                      vcvttss2si edx, xmm0
                    }
                    sub_10178600(a1: _EDX, a2: v1725, a3: 50, a4: 50, a5: v751, a6: 8, a7: (char)i); /*0x100329cf*/
                    v752 = dword_103F4E4C; /*0x100329d4*/
                    if ( dword_103F4E4C != 2 ) /*0x100329dc*/
                      goto LABEL_351; /*0x100329dc*/
LABEL_309:
                    v1990 = i; /*0x10031d7c*/
                    __asm /*0x10031d8b*/
                    {
                      vmovss xmm0, dword ptr [esi+7Ch]
                      vaddss xmm0, xmm0, dword ptr [esi+34h]
                      vaddss xmm0, xmm0, ds:dword_10185854
                      vcvttss2si edx, xmm0
                    }
                    v1725 = _EDX; /*0x10031da1*/
                    __asm /*0x10031da4*/
                    {
                      vmovss xmm0, ds:dword_10184EDC
                      vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                      vaddss xmm0, xmm0, dword ptr [esi+90h]
                      vcvttss2si edx, xmm0
                    }
                    sub_10178600(a1: _EDX, a2: v1725, a3: 50, a4: 50, a5: v751, a6: 8, a7: (char)i); /*0x10031dcc*/
                    v752 = dword_103F4E4C; /*0x10031dd1*/
                    if ( dword_103F4E4C != 1 ) /*0x10031dd9*/
                      goto LABEL_310; /*0x10031dd9*/
LABEL_352:
                    __asm /*0x100329eb*/
                    {
                      vmovss xmm0, ds:dword_10184EDC
                      vmulss xmm0, xmm0, dword ptr [esi+34h]
                      vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                      vcvttss2si eax, xmm0
                    }
                    v1725 = _EAX; /*0x10032a01*/
                    __asm /*0x10032a0a*/
                    {
                      vmovss xmm0, dword ptr [esi+90h]
                      vaddss xmm0, xmm0, dword ptr [esi+0A8h]
                    }
                    v1989 = i; /*0x10032a1d*/
                    __asm /*0x10032a23*/
                    {
                      vaddss xmm0, xmm0, ds:dword_10185854
                      vcvttss2si edx, xmm0
                    }
                    sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v751, a6: 16, a7: (char)i); /*0x10032a3b*/
                    if ( dword_103F4E4C != 3 ) /*0x10032a48*/
                      goto LABEL_290; /*0x10032a48*/
                  }
                  __asm /*0x10032a54*/
                  {
                    vmovss xmm0, ds:dword_10184EDC
                    vmulss xmm0, xmm0, dword ptr [esi+34h]
                    vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                    vcvttss2si eax, xmm0
                  }
                  v1725 = _EAX; /*0x10032a6a*/
                  __asm /*0x10032a6d*/
                  {
                    vmovss xmm0, ds:dword_10185888
                    vaddss xmm0, xmm0, dword ptr [esi+90h]
                  }
                  v1988 = i; /*0x10032a80*/
                  __asm /*0x10032a86*/
                  {
                    vaddss xmm0, xmm0, ds:dword_10185850
                    vcvttss2si edx, xmm0
                  }
                  sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v751, a6: 16, a7: (char)i); /*0x10032a9e*/
                  goto LABEL_290; /*0x10032aa3*/
                }
                if ( dword_103F4DE8 != 1 ) /*0x10031715*/
                  goto LABEL_291; /*0x10031715*/
                v1725 = (char *)sub_10168500(a1: v670); /*0x10031727*/
                if ( byte_103F4DE6 != 1 ) /*0x10031731*/
                  goto LABEL_290; /*0x10031731*/
                sub_1017B960(a1: &v1897); /*0x10031744*/
                v1734 = (wchar_t *)v1898; /*0x1003174f*/
                v672 = dword_103F4E4C; /*0x10031752*/
                if ( dword_103F4E4C != 0 ) /*0x10031759*/
                {
                  if ( dword_103F4E4C == 1 ) /*0x10031762*/
                    goto LABEL_273; /*0x10031762*/
                }
                else
                {
                  v1729 = *(_DWORD *)ArgList; /*0x10032aad*/
                  v1949 = i; /*0x10032ab3*/
                  __asm /*0x10032ab9*/
                  {
                    vmovss xmm0, dword ptr [esi+7Ch]
                    vmovss dword ptr [esi+20h], xmm0
                  }
                  sub_1017B960(a1: &v1897); /*0x10032ad0*/
                  __asm /*0x10032adc*/
                  {
                    vcvtsi2ss xmm0, xmm3, eax
                    vmovss xmm1, dword ptr [esi+20h]
                    vsubss xmm0, xmm1, xmm0
                    vaddss xmm0, xmm0, ds:dword_10185850
                    vcvttss2si eax, xmm0
                  }
                  v1728 = _EAX; /*0x10032af5*/
                  __asm /*0x10032af8*/
                  {
                    vmovss xmm0, ds:dword_10184EDC
                    vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                    vaddss xmm0, xmm0, dword ptr [esi+90h]
                    vcvttss2si ebx, xmm0
                  }
                  sub_1017B960(a1: v1744); /*0x10032b21*/
                  sub_10177250( /*0x10032b40*/
                    a1: v1729,
                    a2: (int)v1744,
                    a3: _EBX,
                    a4: (int)v1728,
                    a5: (char)v1949,
                    a6: 8,
                    Format: v1725,
                    ArgList: v1724[0]);
                  v672 = dword_103F4E4C; /*0x10032b48*/
                  if ( dword_103F4E4C == 1 ) /*0x10032b50*/
                  {
LABEL_273:
                    __asm /*0x10031768*/
                    {
                      vmovss xmm0, ds:dword_10184EDC
                      vmulss xmm0, xmm0, dword ptr [esi+34h]
                      vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                    }
                    __asm { vcvtsi2ss xmm1, xmm3, eax }
                    v1729 = *(_DWORD *)ArgList; /*0x1003178f*/
                    __asm /*0x10031792*/
                    {
                      vsubss xmm0, xmm0, xmm1
                      vcvttss2si eax, xmm0
                    }
                    v1728 = _EAX; /*0x1003179a*/
                    v1948 = i; /*0x100317a0*/
                    __asm /*0x100317a6*/
                    {
                      vmovss xmm0, dword ptr [esi+90h]
                      vaddss xmm0, xmm0, dword ptr [esi+0A8h]
                      vaddss xmm0, xmm0, ds:dword_1018531C
                      vcvttss2si ebx, xmm0
                    }
                    sub_1017B960(a1: &v1897); /*0x100317cf*/
                    sub_10177250( /*0x100317ee*/
                      a1: v1729,
                      a2: (int)&v1897,
                      a3: _EBX,
                      a4: (int)v1728,
                      a5: (char)v1948,
                      a6: 1,
                      Format: v1725,
                      ArgList: v1724[0]);
                    v672 = dword_103F4E4C; /*0x100317f6*/
                    if ( dword_103F4E4C != 2 ) /*0x100317fe*/
                      goto LABEL_274; /*0x100317fe*/
                    goto LABEL_356; /*0x100317fe*/
                  }
                }
                if ( v672 != 2 ) /*0x10032b59*/
                {
LABEL_274:
                  if ( v672 != 3 ) /*0x10031807*/
                    goto LABEL_290; /*0x10031807*/
                  goto LABEL_357; /*0x10031807*/
                }
LABEL_356:
                __asm /*0x10032b5f*/
                {
                  vmovss xmm0, dword ptr [esi+7Ch]
                  vaddss xmm0, xmm0, dword ptr [esi+34h]
                }
                v1729 = *(_DWORD *)ArgList; /*0x10032b6e*/
                __asm /*0x10032b71*/
                {
                  vaddss xmm0, xmm0, ds:dword_10185854
                  vcvttss2si eax, xmm0
                }
                v1728 = _EAX; /*0x10032b7d*/
                __asm /*0x10032b80*/
                {
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                  vaddss xmm0, xmm0, dword ptr [esi+90h]
                }
                v1947 = i; /*0x10032b9b*/
                __asm { vcvttss2si ebx, xmm0 } /*0x10032ba1*/
                sub_1017B960(a1: &v1897); /*0x10032bb2*/
                sub_10177250( /*0x10032bd1*/
                  a1: v1729,
                  a2: (int)&v1897,
                  a3: _EBX,
                  a4: (int)v1728,
                  a5: (char)v1947,
                  a6: 8,
                  Format: v1725,
                  ArgList: v1724[0]);
                if ( dword_103F4E4C != 3 ) /*0x10032be1*/
                {
LABEL_290:
                  v671 = dword_103F4DE8; /*0x10031b9e*/
LABEL_291:
                  if ( v671 != 0 ) /*0x10031ba6*/
                  {
                    if ( v671 == 1 && byte_103F4DE6 == 1 ) /*0x10031bbc*/
                    {
                      v1725 = (char *)&off_101855F8 + 3 * v1727; /*0x10031bcd*/
                      sub_1017B960(a1: &v1897); /*0x10031bdd*/
                      v1734 = (wchar_t *)v1898; /*0x10031be8*/
                      v727 = dword_103F4E4C; /*0x10031beb*/
                      if ( dword_103F4E4C != 0 ) /*0x10031bf2*/
                      {
                        if ( dword_103F4E4C == 1 ) /*0x10031bfb*/
                          goto LABEL_296; /*0x10031bfb*/
                      }
                      else
                      {
                        v1727 = *(_DWORD *)ArgList; /*0x10031df2*/
                        v1738 = *(wchar_t **)v1725; /*0x10031dfa*/
                        v1945 = i; /*0x10031e00*/
                        __asm /*0x10031e06*/
                        {
                          vmovss xmm0, dword ptr [esi+7Ch]
                          vmovss dword ptr [esi+24h], xmm0
                        }
                        sub_1017B960(a1: &v1897); /*0x10031e1d*/
                        __asm /*0x10031e29*/
                        {
                          vcvtsi2ss xmm0, xmm3, eax
                          vmovss xmm1, dword ptr [esi+24h]
                          vsubss xmm0, xmm1, xmm0
                          vaddss xmm0, xmm0, ds:dword_10185850
                          vcvttss2si ebx, xmm0
                          vmovss xmm0, ds:dword_10184EDC
                          vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                          vaddss xmm0, xmm0, dword ptr [esi+90h]
                          vcvttss2si edi, xmm0
                        }
                        sub_1017B960(a1: v1744); /*0x10031e6b*/
                        sub_10177410( /*0x10031e88*/
                          a1: v1727,
                          a2: (int)v1744,
                          a3: _EDI,
                          a4: _EBX,
                          a5: (char)v1945,
                          a6: 8,
                          Format: v1738,
                          ArgList: v1724[0]);
                        v727 = dword_103F4E4C; /*0x10031e90*/
                        if ( dword_103F4E4C == 1 ) /*0x10031e98*/
                        {
LABEL_296:
                          __asm /*0x10031c01*/
                          {
                            vmovss xmm0, ds:dword_10184EDC
                            vmulss xmm0, xmm0, dword ptr [esi+34h]
                            vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                          }
                          v1727 = *(_DWORD *)ArgList; /*0x10031c18*/
                          __asm { vcvtsi2ss xmm1, xmm3, eax } /*0x10031c27*/
                          v732 = *(wchar_t **)v1725; /*0x10031c2e*/
                          __asm /*0x10031c30*/
                          {
                            vsubss xmm0, xmm0, xmm1
                            vcvttss2si eax, xmm0
                          }
                          v1738 = _EAX; /*0x10031c38*/
                          v1944 = i; /*0x10031c3e*/
                          __asm /*0x10031c44*/
                          {
                            vmovss xmm0, dword ptr [esi+90h]
                            vaddss xmm0, xmm0, dword ptr [esi+0A8h]
                            vaddss xmm0, xmm0, ds:dword_1018531C
                            vcvttss2si ebx, xmm0
                          }
                          sub_1017B960(a1: &v1897); /*0x10031c6d*/
                          sub_10177410( /*0x10031c8a*/
                            a1: v1727,
                            a2: (int)&v1897,
                            a3: _EBX,
                            a4: (int)v1738,
                            a5: (char)v1944,
                            a6: 1,
                            Format: v732,
                            ArgList: v1724[0]);
                          v727 = dword_103F4E4C; /*0x10031c92*/
                          if ( dword_103F4E4C != 2 ) /*0x10031c9a*/
                            goto LABEL_297; /*0x10031c9a*/
                          goto LABEL_314; /*0x10031c9a*/
                        }
                      }
                      if ( v727 != 2 ) /*0x10031ea1*/
                      {
LABEL_297:
                        if ( v727 != 3 ) /*0x10031ca3*/
                          goto LABEL_341; /*0x10031ca3*/
LABEL_315:
                        v1727 = *(_DWORD *)ArgList; /*0x10031f32*/
                        v1725 = *(char **)v1725; /*0x10031f3f*/
                        v1942 = i; /*0x10031f45*/
                        __asm /*0x10031f4b*/
                        {
                          vmovss xmm0, ds:dword_10184EDC
                          vmulss xmm0, xmm0, dword ptr [esi+34h]
                          vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                        }
                        __asm
                        {
                          vcvtsi2ss xmm1, xmm3, eax
                          vsubss xmm0, xmm0, xmm1
                          vcvttss2si edi, xmm0
                          vmovss xmm0, ds:dword_10185884
                          vaddss xmm0, xmm0, dword ptr [esi+90h]
                          vaddss xmm0, xmm0, ds:dword_1018550C
                          vcvttss2si ebx, xmm0
                        }
                        sub_1017B960(a1: &v1897); /*0x10031f9e*/
                        sub_10177410( /*0x10031fbb*/
                          a1: v1727,
                          a2: (int)&v1897,
                          a3: _EBX,
                          a4: _EDI,
                          a5: (char)v1942,
                          a6: 1,
                          Format: (wchar_t *)v1725,
                          ArgList: v1724[0]);
                        goto LABEL_341; /*0x10031fc3*/
                      }
LABEL_314:
                      v1727 = *(_DWORD *)ArgList; /*0x10031ea7*/
                      __asm /*0x10031eaf*/
                      {
                        vmovss xmm0, dword ptr [esi+7Ch]
                        vaddss xmm0, xmm0, dword ptr [esi+34h]
                      }
                      v773 = *(wchar_t **)v1725; /*0x10031ebc*/
                      __asm /*0x10031ebe*/
                      {
                        vaddss xmm0, xmm0, ds:dword_10185854
                        vcvttss2si eax, xmm0
                      }
                      v1738 = _EAX; /*0x10031eca*/
                      __asm /*0x10031ecd*/
                      {
                        vmovss xmm0, ds:dword_10184EDC
                        vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                        vaddss xmm0, xmm0, dword ptr [esi+90h]
                      }
                      v1943 = i; /*0x10031ee8*/
                      __asm { vcvttss2si ebx, xmm0 } /*0x10031eee*/
                      sub_1017B960(a1: &v1897); /*0x10031eff*/
                      sub_10177410( /*0x10031f1c*/
                        a1: v1727,
                        a2: (int)&v1897,
                        a3: _EBX,
                        a4: (int)v1738,
                        a5: (char)v1943,
                        a6: 8,
                        Format: v773,
                        ArgList: v1724[0]);
                      if ( dword_103F4E4C != 3 ) /*0x10031f2c*/
                        goto LABEL_341; /*0x10031f2c*/
                      goto LABEL_315; /*0x10031f2c*/
                    }
LABEL_341:
                    if ( byte_103F4DE5 == 1 ) /*0x10032633*/
                    {
                      if ( dword_103F4DEC == 1 ) /*0x10032641*/
                      {
                        __asm { vcvttss2si eax, dword ptr [esi+34h] } /*0x10030d6b*/
                        v1725 = _EAX; /*0x10030d70*/
                        __asm { vcvttss2si edx, dword ptr [esi+0A8h] } /*0x10030d73*/
                        __asm { vcvttss2si edi, dword ptr [esi+7Ch] }
                        v1982 = i; /*0x10030d89*/
                        __asm { vcvttss2si ebx, dword ptr [esi+90h] } /*0x10030d8f*/
                        sub_101778E0(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: (char)i, a6: 1); /*0x10030da0*/
                      }
                      else if ( dword_103F4DEC == 0 ) /*0x10032649*/
                      {
                        __asm /*0x1003264f*/
                        {
                          vmovss xmm0, dword ptr [esi+34h]
                          vmovss dword ptr [esi+40h], xmm0
                          vcvttss2si eax, xmm0
                        }
                        v1729 = _EAX; /*0x1003265d*/
                        __asm /*0x10032660*/
                        {
                          vmovss xmm0, dword ptr [esi+0A8h]
                          vmovss dword ptr [esi+1Ch], xmm0
                          vcvttss2si edx, xmm0
                        }
                        __asm
                        {
                          vmovss xmm0, dword ptr [esi+7Ch]
                          vmovss dword ptr [esi+14h], xmm0
                          vcvttss2si edi, xmm0
                        }
                        LOBYTE(_EAX) = (_BYTE)i; /*0x10032685*/
                        v1983 = i; /*0x10032688*/
                        __asm /*0x1003268e*/
                        {
                          vmovss xmm0, dword ptr [esi+90h]
                          vmovss dword ptr [esi+60h], xmm0
                          vcvttss2si ebx, xmm0
                        }
                        sub_10177840(a1: _EBX, a2: _EDI, a3: _EDX, a4: v1729, a5: _EAX, a6: 1); /*0x100326a8*/
                        __asm /*0x100326ad*/
                        {
                          vmovss xmm1, ds:dword_10185854
                          vaddss xmm0, xmm1, dword ptr [esi+40h]
                          vcvttss2si eax, xmm0
                        }
                        i = _EAX; /*0x100326be*/
                        __asm /*0x100326c1*/
                        {
                          vaddss xmm0, xmm1, dword ptr [esi+1Ch]
                          vcvttss2si edx, xmm0
                        }
                        __asm
                        {
                          vmovss xmm1, ds:dword_1018555C
                          vaddss xmm0, xmm1, dword ptr [esi+14h]
                          vcvttss2si edi, xmm0
                        }
                        v1941 = -16777216; /*0x100326e1*/
                        __asm /*0x100326eb*/
                        {
                          vaddss xmm0, xmm1, dword ptr [esi+60h]
                          vcvttss2si ebx, xmm0
                        }
                        sub_10177840(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: 0, a6: 1); /*0x10032703*/
                        __asm /*0x10032708*/
                        {
                          vmovss xmm1, ds:dword_10185850
                          vaddss xmm0, xmm1, dword ptr [esi+40h]
                          vcvttss2si eax, xmm0
                        }
                        i = _EAX; /*0x10032719*/
                        __asm /*0x1003271c*/
                        {
                          vaddss xmm0, xmm1, dword ptr [esi+1Ch]
                          vcvttss2si edx, xmm0
                        }
                        __asm
                        {
                          vmovss xmm1, ds:dword_10184184
                          vaddss xmm0, xmm1, dword ptr [esi+14h]
                          vcvttss2si edi, xmm0
                        }
                        v1940 = -16777216; /*0x1003273c*/
                        __asm /*0x10032746*/
                        {
                          vaddss xmm0, xmm1, dword ptr [esi+60h]
                          vcvttss2si ebx, xmm0
                        }
                        sub_10177840(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: 0, a6: 1); /*0x1003275e*/
                      }
                    }
                    goto LABEL_209; /*0x10032763*/
                  }
                  if ( v1738 == (wchar_t *)2 ) /*0x10031cb2*/
                    goto LABEL_341; /*0x10031cb2*/
                  v739 = sub_10168E50(a1: v512); /*0x10031cbf*/
                  if ( v739 == 0 ) /*0x10031cc6*/
                    goto LABEL_341; /*0x10031cc6*/
                  v740 = v739; /*0x10031ccc*/
                  v741 = dword_103F4E4C; /*0x10031cce*/
                  if ( dword_103F4E4C != 0 ) /*0x10031cd5*/
                  {
                    if ( dword_103F4E4C == 2 ) /*0x10031cde*/
                      goto LABEL_303; /*0x10031cde*/
                  }
                  else
                  {
                    v1987 = i; /*0x10032521*/
                    __asm /*0x10032527*/
                    {
                      vmovss xmm0, ds:dword_10185888
                      vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                      vaddss xmm0, xmm0, ds:dword_10185850
                      vcvttss2si eax, xmm0
                      vmovss xmm0, ds:dword_10184EDC
                      vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                      vaddss xmm0, xmm0, dword ptr [esi+90h]
                      vcvttss2si edx, xmm0
                    }
                    sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v740, a6: 8, a7: (char)i); /*0x10032566*/
                    v741 = dword_103F4E4C; /*0x1003256b*/
                    if ( dword_103F4E4C == 2 ) /*0x10032573*/
                    {
LABEL_303:
                      v1986 = i; /*0x10031ce4*/
                      __asm /*0x10031cf3*/
                      {
                        vmovss xmm0, dword ptr [esi+7Ch]
                        vaddss xmm0, xmm0, dword ptr [esi+34h]
                        vaddss xmm0, xmm0, ds:dword_10185854
                        vcvttss2si eax, xmm0
                        vmovss xmm0, ds:dword_10184EDC
                        vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                        vaddss xmm0, xmm0, dword ptr [esi+90h]
                        vcvttss2si edx, xmm0
                      }
                      sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v740, a6: 8, a7: (char)i); /*0x10031d2f*/
                      v741 = dword_103F4E4C; /*0x10031d34*/
                      if ( dword_103F4E4C != 1 ) /*0x10031d3c*/
                        goto LABEL_304; /*0x10031d3c*/
                      goto LABEL_339; /*0x10031d3c*/
                    }
                  }
                  if ( v741 != 1 ) /*0x1003257c*/
                  {
LABEL_304:
                    if ( v741 != 3 ) /*0x10031d45*/
                      goto LABEL_341; /*0x10031d45*/
LABEL_340:
                    __asm /*0x100325dc*/
                    {
                      vmovss xmm0, ds:dword_10184EDC
                      vmulss xmm0, xmm0, dword ptr [esi+34h]
                      vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                      vcvttss2si eax, xmm0
                      vmovss xmm0, ds:dword_10185888
                      vaddss xmm0, xmm0, dword ptr [esi+90h]
                    }
                    v1984 = i; /*0x1003260b*/
                    __asm /*0x10032611*/
                    {
                      vaddss xmm0, xmm0, ds:dword_10185850
                      vcvttss2si edx, xmm0
                    }
                    sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v740, a6: 16, a7: (char)i); /*0x10032627*/
                    goto LABEL_341; /*0x10032627*/
                  }
LABEL_339:
                  __asm /*0x10032582*/
                  {
                    vmovss xmm0, ds:dword_10184EDC
                    vmulss xmm0, xmm0, dword ptr [esi+34h]
                    vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                    vcvttss2si eax, xmm0
                  }
                  __asm
                  {
                    vmovss xmm0, dword ptr [esi+90h]
                    vaddss xmm0, xmm0, dword ptr [esi+0A8h]
                  }
                  v1985 = i; /*0x100325b1*/
                  __asm /*0x100325b7*/
                  {
                    vaddss xmm0, xmm0, ds:dword_10185854
                    vcvttss2si edx, xmm0
                  }
                  sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v740, a6: 16, a7: (char)i); /*0x100325cd*/
                  if ( dword_103F4E4C != 3 ) /*0x100325da*/
                    goto LABEL_341; /*0x100325da*/
                  goto LABEL_340; /*0x100325da*/
                }
LABEL_357:
                v1729 = *(_DWORD *)ArgList; /*0x10032be7*/
                v1946 = i; /*0x10032bf2*/
                __asm /*0x10032bf8*/
                {
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+34h]
                  vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                }
                __asm
                {
                  vcvtsi2ss xmm1, xmm3, eax
                  vsubss xmm0, xmm0, xmm1
                  vcvttss2si eax, xmm0
                }
                v1734 = (wchar_t *)_EAX; /*0x10032c22*/
                __asm /*0x10032c25*/
                {
                  vmovss xmm0, ds:dword_10185884
                  vaddss xmm0, xmm0, dword ptr [esi+90h]
                  vaddss xmm0, xmm0, ds:dword_1018550C
                  vcvttss2si ebx, xmm0
                }
                sub_1017B960(a1: &v1897); /*0x10032c4e*/
                sub_10177250( /*0x10032c6d*/
                  a1: v1729,
                  a2: (int)&v1897,
                  a3: _EBX,
                  a4: (int)v1734,
                  a5: (char)v1946,
                  a6: 1,
                  Format: v1725,
                  ArgList: v1724[0]);
                goto LABEL_290; /*0x10032c75*/
              case 3u: /*0x100316cd*/
                *(_DWORD *)&v1723[16] = &xmmword_103F4E1C; /*0x10031b87*/
                goto LABEL_289; /*0x10031b8c*/
              case 4u: /*0x100316cd*/
                *(_DWORD *)&v1723[16] = &xmmword_103F4E2C; /*0x100316de*/
LABEL_289:
                i = (char **)sub_1002DFA0(a1: *(_DWORD *)&v1723[16]); /*0x10031b93*/
                goto LABEL_290; /*0x10031b9b*/
              default:
                goto LABEL_209;
            }
          }
          goto LABEL_209; /*0x10030e58*/
        }
        if ( (unsigned __int8)sub_1002E000(a1: v502, a2: v1741, a3: &v1739, a4: v1742, a5: v1732) == 0 ) /*0x1003117b*/
          goto LABEL_209; /*0x1003117b*/
        (*(void (__thiscall **)(char *))(*(_DWORD *)v1748 + 44))(a1: v1748); /*0x10031189*/
        __asm /*0x1003118c*/
        {
          vmovss xmm0, dword ptr [esi+40h]
          vsubss xmm0, xmm0, dword ptr [eax+8]
          vmovss xmm1, dword ptr [esi+14h]
          vsubss xmm1, xmm1, dword ptr [eax+4]
          vmovss xmm2, dword ptr [esi+18h]
          vsubss xmm2, xmm2, dword ptr [eax]
          vmulss xmm1, xmm1, xmm1
          vfmadd231ss xmm1, xmm2, xmm2
          vfmadd231ss xmm1, xmm0, xmm0
          vcvtss2sd xmm0, xmm1, xmm1
          vsqrtsd xmm0, xmm0, xmm0
          vcvtsd2ss xmm0, xmm0, xmm0
          vucomiss xmm0, dword_103F4F74
        }
        if ( !(v89 | v90) || byte_103F4F50 != 1 || byte_103F4F51 != 1 ) /*0x100311e5*/
          goto LABEL_209; /*0x100311e5*/
        __asm /*0x100311eb*/
        {
          vmovss xmm1, ds:dword_10184380
          vmulss xmm0, xmm1, dword ptr xmmword_103F4F78
          vcvttss2si ebx, xmm0
          vmulss xmm0, xmm1, dword ptr xmmword_103F4F78+4
          vcvttss2si eax, xmm0
        }
        i = _EAX; /*0x1003120b*/
        __asm /*0x1003120e*/
        {
          vmulss xmm0, xmm1, dword ptr xmmword_103F4F78+8
          vcvttss2si eax, xmm0
        }
        v1725 = _EAX; /*0x1003121a*/
        if ( byte_103F4F52 == 1 ) /*0x10031224*/
        {
          if ( dword_103F4F54 == 1 ) /*0x10031232*/
          {
            v1789 = _EBX; /*0x10031818*/
            __asm { vcvttss2si eax, dword ptr [esi+34h] } /*0x1003181e*/
            v1734 = (wchar_t *)_EAX; /*0x10031823*/
            v1790 = (char)i; /*0x10031829*/
            __asm { vcvttss2si eax, dword ptr [esi+0A8h] } /*0x1003182f*/
            v1727 = _EAX; /*0x10031837*/
            v1791 = (char)v1725; /*0x1003183d*/
            __asm { vcvttss2si edi, dword ptr [esi+7Ch] } /*0x10031843*/
            v1792 = -1; /*0x10031848*/
            v686 = _EBX; /*0x1003184f*/
            __asm { vcvttss2si ebx, dword ptr [esi+90h] } /*0x10031851*/
            v1722 = _EBX; /*0x10031869*/
            _EBX = v686; /*0x1003186a*/
            sub_101778E0(a1: v1722, a2: _EDI, a3: _EAX, a4: v1734, a5: v1789, a6: 1); /*0x1003186c*/
          }
          else if ( dword_103F4F54 == 0 ) /*0x1003123a*/
          {
            LOBYTE(v1793) = _EBX; /*0x10031246*/
            __asm /*0x1003124c*/
            {
              vmovss xmm0, dword ptr [esi+34h]
              vmovss dword ptr [esi+40h], xmm0
              vcvttss2si eax, xmm0
            }
            v1731 = _EAX; /*0x1003125a*/
            BYTE1(v1793) = (_BYTE)i; /*0x10031260*/
            __asm /*0x10031266*/
            {
              vmovss xmm0, dword ptr [esi+0A8h]
              vmovss dword ptr [esi+60h], xmm0
              vcvttss2si edx, xmm0
            }
            BYTE2(v1793) = (_BYTE)v1725; /*0x1003127a*/
            __asm /*0x10031280*/
            {
              vmovss xmm0, dword ptr [esi+7Ch]
              vmovss dword ptr [esi+24h], xmm0
              vcvttss2si edi, xmm0
            }
            HIBYTE(v1793) = -1; /*0x1003128e*/
            __asm /*0x10031295*/
            {
              vmovss xmm0, dword ptr [esi+90h]
              vmovss dword ptr [esi+20h], xmm0
            }
            v1727 = _EBX; /*0x100312a2*/
            __asm { vcvttss2si ebx, xmm0 } /*0x100312a5*/
            sub_101777E0(a1: _EBX, a2: _EDI, a3: _EDX, a4: v1731, a5: v1793); /*0x100312b6*/
            __asm /*0x100312bb*/
            {
              vmovss xmm1, ds:dword_10185854
              vaddss xmm0, xmm1, dword ptr [esi+40h]
              vcvttss2si eax, xmm0
            }
            v1731 = _EAX; /*0x100312cc*/
            __asm /*0x100312cf*/
            {
              vaddss xmm0, xmm1, dword ptr [esi+60h]
              vcvttss2si edx, xmm0
            }
            __asm
            {
              vmovss xmm1, ds:dword_1018555C
              vaddss xmm0, xmm1, dword ptr [esi+24h]
              vcvttss2si edi, xmm0
            }
            v1937 = -16777216; /*0x100312ef*/
            __asm /*0x100312f9*/
            {
              vaddss xmm0, xmm1, dword ptr [esi+20h]
              vcvttss2si ebx, xmm0
            }
            sub_101777E0(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: -16777216); /*0x1003130f*/
            __asm /*0x10031314*/
            {
              vmovss xmm1, ds:dword_10185850
              vaddss xmm0, xmm1, dword ptr [esi+40h]
              vcvttss2si eax, xmm0
            }
            v1734 = (wchar_t *)_EAX; /*0x10031325*/
            __asm /*0x10031328*/
            {
              vaddss xmm0, xmm1, dword ptr [esi+60h]
              vcvttss2si edx, xmm0
            }
            __asm
            {
              vmovss xmm1, ds:dword_10184184
              vaddss xmm0, xmm1, dword ptr [esi+24h]
              vcvttss2si edi, xmm0
            }
            v1936 = -16777216; /*0x10031348*/
            __asm /*0x10031352*/
            {
              vaddss xmm0, xmm1, dword ptr [esi+20h]
              vcvttss2si ebx, xmm0
            }
            *(_DWORD *)v1723 = _EBX; /*0x10031367*/
            _EBX = v1727; /*0x10031368*/
            sub_101777E0(a1: *(_DWORD *)v1723, a2: _EDI, a3: _EDX, a4: _EAX, a5: -16777216); /*0x1003136b*/
          }
        }
        if ( byte_103F4F53 != 1 ) /*0x10031878*/
          goto LABEL_209; /*0x10031878*/
        sub_1017B960(a1: &v1897); /*0x1003188b*/
        v1734 = (wchar_t *)v1898; /*0x10031896*/
        v688 = dword_103F4F58; /*0x10031899*/
        if ( dword_103F4F58 != 0 ) /*0x100318a0*/
        {
          if ( dword_103F4F58 == 1 ) /*0x100318a9*/
            goto LABEL_280; /*0x100318a9*/
        }
        else
        {
          v1727 = *(_DWORD *)ArgList; /*0x10031977*/
          v1785 = _EBX; /*0x1003197a*/
          v1786 = (char)i; /*0x10031983*/
          v1787 = (char)v1725; /*0x1003198c*/
          v1788 = -1; /*0x10031992*/
          __asm /*0x10031999*/
          {
            vmovss xmm0, dword ptr [esi+7Ch]
            vmovss dword ptr [esi+60h], xmm0
          }
          sub_1017B960(a1: &v1897); /*0x100319b0*/
          __asm /*0x100319bc*/
          {
            vcvtsi2ss xmm0, xmm3, eax
            vmovss xmm1, dword ptr [esi+60h]
            vsubss xmm0, xmm1, xmm0
            vaddss xmm0, xmm0, ds:dword_10185850
            vcvttss2si eax, xmm0
          }
          v1738 = _EAX; /*0x100319d5*/
          __asm /*0x100319d8*/
          {
            vmovss xmm0, ds:dword_10184EDC
            vmulss xmm0, xmm0, dword ptr [esi+0A8h]
            vaddss xmm0, xmm0, dword ptr [esi+90h]
          }
          v707 = _EBX; /*0x100319f0*/
          __asm { vcvttss2si ebx, xmm0 } /*0x100319f2*/
          sub_1017B960(a1: v1744); /*0x10031a03*/
          *(_DWORD *)v1723 = _EBX; /*0x10031a19*/
          _EBX = v707; /*0x10031a1a*/
          sub_10177250( /*0x10031a26*/
            a1: v1727,
            a2: (int)v1744,
            a3: *(int *)v1723,
            a4: (int)v1738,
            a5: v1785,
            a6: 8,
            Format: (char *)"Rock",
            ArgList: v1724[0]);
          v688 = dword_103F4F58; /*0x10031a2e*/
          if ( dword_103F4F58 == 1 ) /*0x10031a36*/
          {
LABEL_280:
            v1738 = *(wchar_t **)ArgList; /*0x100318af*/
            v1781 = _EBX; /*0x100318b7*/
            __asm /*0x100318bd*/
            {
              vmovss xmm0, ds:dword_10184EDC
              vmulss xmm0, xmm0, dword ptr [esi+34h]
              vaddss xmm0, xmm0, dword ptr [esi+7Ch]
            }
            v1782 = (char)i; /*0x100318d2*/
            __asm { vcvtsi2ss xmm1, xmm3, eax } /*0x100318e4*/
            v1783 = (char)v1725; /*0x100318eb*/
            __asm { vsubss xmm0, xmm0, xmm1 } /*0x100318f1*/
            v1727 = _EBX; /*0x100318f5*/
            __asm { vcvttss2si ebx, xmm0 } /*0x100318f8*/
            v1784 = -1; /*0x100318fc*/
            __asm /*0x10031903*/
            {
              vmovss xmm0, dword ptr [esi+90h]
              vaddss xmm0, xmm0, dword ptr [esi+0A8h]
              vaddss xmm0, xmm0, ds:dword_1018531C
              vcvttss2si edi, xmm0
            }
            sub_1017B960(a1: &v1897); /*0x1003192c*/
            *(_DWORD *)&v1723[4] = _EBX; /*0x1003193f*/
            LOBYTE(_EBX) = v1727; /*0x10031940*/
            sub_10177250( /*0x1003194e*/
              a1: (int)v1738,
              a2: (int)&v1897,
              a3: _EDI,
              a4: *(int *)&v1723[4],
              a5: v1781,
              a6: 1,
              Format: (char *)"Rock",
              ArgList: v1724[0]);
            v688 = dword_103F4F58; /*0x10031956*/
            if ( dword_103F4F58 != 2 ) /*0x1003195e*/
              goto LABEL_281; /*0x1003195e*/
            goto LABEL_285; /*0x1003195e*/
          }
        }
        if ( v688 != 2 ) /*0x10031a3f*/
        {
LABEL_281:
          if ( v688 == 3 ) /*0x10031967*/
            goto LABEL_286; /*0x10031967*/
          goto LABEL_209; /*0x10031967*/
        }
LABEL_285:
        v1727 = *(_DWORD *)ArgList; /*0x10031a45*/
        v1777 = _EBX; /*0x10031a4d*/
        v1778 = (char)i; /*0x10031a56*/
        __asm /*0x10031a5c*/
        {
          vmovss xmm0, dword ptr [esi+7Ch]
          vaddss xmm0, xmm0, dword ptr [esi+34h]
        }
        v1779 = (char)v1725; /*0x10031a69*/
        __asm /*0x10031a6f*/
        {
          vaddss xmm0, xmm0, ds:dword_10185854
          vcvttss2si eax, xmm0
        }
        v1738 = _EAX; /*0x10031a7b*/
        __asm /*0x10031a7e*/
        {
          vmovss xmm0, ds:dword_10184EDC
          vmulss xmm0, xmm0, dword ptr [esi+0A8h]
          vaddss xmm0, xmm0, dword ptr [esi+90h]
        }
        v1780 = -1; /*0x10031a96*/
        v716 = _EBX; /*0x10031a9d*/
        __asm { vcvttss2si ebx, xmm0 } /*0x10031a9f*/
        sub_1017B960(a1: &v1897); /*0x10031ab0*/
        *(_DWORD *)v1723 = _EBX; /*0x10031ac6*/
        LOBYTE(_EBX) = v716; /*0x10031ac7*/
        sub_10177250( /*0x10031ad3*/
          a1: v1727,
          a2: (int)&v1897,
          a3: *(int *)v1723,
          a4: (int)v1738,
          a5: v1777,
          a6: 8,
          Format: (char *)"Rock",
          ArgList: v1724[0]);
        if ( dword_103F4F58 == 3 ) /*0x10031ae3*/
        {
LABEL_286:
          v1773 = _EBX; /*0x10031ae9*/
          v1727 = *(_DWORD *)ArgList; /*0x10031af4*/
          v1774 = (char)i; /*0x10031afa*/
          v1775 = (char)v1725; /*0x10031b03*/
          v1776 = -1; /*0x10031b09*/
          __asm /*0x10031b10*/
          {
            vmovss xmm0, ds:dword_10184EDC
            vmulss xmm0, xmm0, dword ptr [esi+34h]
            vaddss xmm0, xmm0, dword ptr [esi+7Ch]
          }
          __asm
          {
            vcvtsi2ss xmm1, xmm3, eax
            vsubss xmm0, xmm0, xmm1
            vcvttss2si edi, xmm0
            vmovss xmm0, ds:dword_10185884
            vaddss xmm0, xmm0, dword ptr [esi+90h]
            vaddss xmm0, xmm0, ds:dword_1018550C
            vcvttss2si ebx, xmm0
          }
          sub_1017B960(a1: &v1897); /*0x10031b63*/
          sub_10177250( /*0x10031b82*/
            a1: v1727,
            a2: (int)&v1897,
            a3: _EBX,
            a4: _EDI,
            a5: v1773,
            a6: 1,
            Format: (char *)"Rock",
            ArgList: v1724[0]);
        }
LABEL_209:
        v499 = (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC7B8 + 32))(a1: dword_103EC7B8); /*0x10030db0*/
        v494 = v1730; /*0x10030dbb*/
      }
      while ( (int)v1730 < v499 ); /*0x10030dcd*/
    }
  }
  if ( byte_103F4DE4 == 1 ) /*0x10032c81*/
  {
    v992 = *(void (__thiscall **)(int, int))(*(_DWORD *)dword_103EC728 + 420); /*0x10032c8b*/
    v2004 = -1; /*0x10032c91*/
    v992(a1: dword_103EC728, a2: 1065353216); /*0x10032ca0*/
    v1741[0] = 0.0; /*0x10032ca2*/
    v1739 = 0; /*0x10032cac*/
    v1742[0] = 0; /*0x10032cb3*/
    v1732[0] = 0; /*0x10032cbd*/
    v1727 = sub_1002DFA0(a1: &xmmword_103F4E2C); /*0x10032cd1*/
    v993 = (wchar_t **)sub_10029F30(a1: 3); /*0x10032cdc*/
    v994 = *v993; /*0x10032ce1*/
    v1734 = v993[1]; /*0x10032ce6*/
    if ( v994 != v1734 ) /*0x10032ceb*/
    {
      while ( 1 ) /*0x10032dec*/
      {
        v1010 = *(_DWORD *)v994; /*0x10032dec*/
        _EAX = (*(int (__thiscall **)(_DWORD))(**(_DWORD **)v994 + 44))(a1: *(_DWORD *)v994); /*0x10032df2*/
        __asm /*0x10032df5*/
        {
          vmovss xmm0, dword ptr [eax]
          vmovss dword ptr [esi+28h], xmm0
          vmovss xmm0, dword ptr [eax+4]
          vmovss dword ptr [esi+18h], xmm0
          vmovss xmm0, dword ptr [eax+8]
          vmovss dword ptr [esi+14h], xmm0
        }
        if ( (unsigned __int8)sub_1002E000(a1: v1010, a2: v1741, a3: &v1739, a4: v1742, a5: v1732) != 0 ) /*0x10032e30*/
        {
          (*(void (__thiscall **)(char *))(*(_DWORD *)v1748 + 44))(a1: v1748); /*0x10032e3a*/
          __asm /*0x10032e3d*/
          {
            vmovss xmm0, dword ptr [esi+14h]
            vsubss xmm0, xmm0, dword ptr [eax+8]
            vmovss xmm1, dword ptr [esi+18h]
            vsubss xmm1, xmm1, dword ptr [eax+4]
            vmovss xmm2, dword ptr [esi+28h]
            vsubss xmm2, xmm2, dword ptr [eax]
            vmulss xmm1, xmm1, xmm1
            vfmadd231ss xmm1, xmm2, xmm2
            vfmadd231ss xmm1, xmm0, xmm0
            vcvtss2sd xmm0, xmm1, xmm1
            vsqrtsd xmm0, xmm0, xmm0
            vcvtsd2ss xmm0, xmm0, xmm0
            vucomiss xmm0, dword_103F4DF8
          }
          if ( v89 | v90 ) /*0x10032e7c*/
          {
            if ( byte_103F4DF3 == 1 ) /*0x10032e89*/
              break; /*0x10032e89*/
          }
        }
LABEL_365:
        v994 += 2; /*0x10032de0*/
        if ( v994 == v1734 ) /*0x10032de6*/
          goto LABEL_360; /*0x10032de6*/
      }
      if ( byte_103F4DE6 != 1 ) /*0x10032e96*/
      {
        if ( dword_103F4DE8 == 0 ) /*0x10032f80*/
        {
          __asm /*0x10032f91*/
          {
            vmovss xmm0, ds:dword_1018588C
            vaddss xmm0, xmm0, dword ptr [esi+7Ch]
          }
          v1040 = sub_1016A430(a1: off_1029E57C); /*0x10032f9e*/
          __asm /*0x10032fa0*/
          {
            vaddss xmm0, xmm0, ds:dword_10185850
            vcvttss2si eax, xmm0
          }
          v1730 = _EAX; /*0x10032fac*/
          v1931 = v1727; /*0x10032fb8*/
          __asm /*0x10032fbe*/
          {
            vmovss xmm0, ds:dword_10184EDC
            vmulss xmm0, xmm0, dword ptr [esi+0A8h]
            vaddss xmm0, xmm0, dword ptr [esi+90h]
            vcvttss2si ebx, xmm0
          }
          sub_10178600(a1: _EBX, a2: _EAX, a3: 50, a4: 50, a5: v1040, a6: 8, a7: v1727); /*0x10032fec*/
        }
        goto LABEL_381; /*0x10032ff1*/
      }
      sub_1017B960(a1: &v1897); /*0x10032ea9*/
      v1730 = v1898; /*0x10032eb4*/
      v1027 = dword_103F4E4C; /*0x10032eb7*/
      if ( dword_103F4E4C != 0 ) /*0x10032ebe*/
      {
        if ( dword_103F4E4C == 1 ) /*0x10032ec7*/
          goto LABEL_372; /*0x10032ec7*/
      }
      else
      {
        i = *(char ***)ArgList; /*0x10032ffb*/
        v1935 = v1727; /*0x10033001*/
        __asm /*0x10033007*/
        {
          vmovss xmm0, dword ptr [esi+7Ch]
          vmovss dword ptr [esi+14h], xmm0
        }
        sub_1017B960(a1: &v1897); /*0x1003301e*/
        __asm /*0x1003302a*/
        {
          vcvtsi2ss xmm0, xmm3, eax
          vmovss xmm1, dword ptr [esi+14h]
          vsubss xmm0, xmm1, xmm0
          vaddss xmm0, xmm0, ds:dword_10185850
          vcvttss2si eax, xmm0
        }
        v1725 = _EAX; /*0x10033043*/
        __asm /*0x10033046*/
        {
          vmovss xmm0, ds:dword_10184EDC
          vmulss xmm0, xmm0, dword ptr [esi+0A8h]
          vaddss xmm0, xmm0, dword ptr [esi+90h]
          vcvttss2si ebx, xmm0
        }
        sub_1017B960(a1: v1744); /*0x1003306f*/
        sub_10177250( /*0x10033090*/
          a1: (int)i,
          a2: (int)v1744,
          a3: _EBX,
          a4: (int)v1725,
          a5: v1935,
          a6: 8,
          Format: (char *)"Ammunition",
          ArgList: v1724[0]);
        v1027 = dword_103F4E4C; /*0x10033098*/
        if ( dword_103F4E4C == 1 ) /*0x100330a0*/
        {
LABEL_372:
          __asm /*0x10032ecd*/
          {
            vmovss xmm0, ds:dword_10184EDC
            vmulss xmm0, xmm0, dword ptr [esi+34h]
            vaddss xmm0, xmm0, dword ptr [esi+7Ch]
          }
          __asm { vcvtsi2ss xmm1, xmm3, eax }
          i = *(char ***)ArgList; /*0x10032ef4*/
          __asm /*0x10032ef7*/
          {
            vsubss xmm0, xmm0, xmm1
            vcvttss2si eax, xmm0
          }
          v1725 = _EAX; /*0x10032eff*/
          v1934 = v1727; /*0x10032f05*/
          __asm /*0x10032f0b*/
          {
            vmovss xmm0, dword ptr [esi+90h]
            vaddss xmm0, xmm0, dword ptr [esi+0A8h]
            vaddss xmm0, xmm0, ds:dword_1018531C
            vcvttss2si ebx, xmm0
          }
          sub_1017B960(a1: &v1897); /*0x10032f34*/
          sub_10177250( /*0x10032f55*/
            a1: (int)i,
            a2: (int)&v1897,
            a3: _EBX,
            a4: (int)v1725,
            a5: v1934,
            a6: 1,
            Format: (char *)"Ammunition",
            ArgList: v1724[0]);
          v1027 = dword_103F4E4C; /*0x10032f5d*/
          if ( dword_103F4E4C != 2 ) /*0x10032f65*/
            goto LABEL_373; /*0x10032f65*/
          goto LABEL_379; /*0x10032f65*/
        }
      }
      if ( v1027 != 2 ) /*0x100330a9*/
      {
LABEL_373:
        if ( v1027 != 3 ) /*0x10032f6e*/
        {
LABEL_381:
          if ( byte_103F4DE5 == 1 ) /*0x100331d0*/
          {
            if ( dword_103F4DEC == 1 ) /*0x100331de*/
            {
              __asm { vcvttss2si eax, dword ptr [esi+34h] } /*0x10032da0*/
              v1730 = _EAX; /*0x10032da5*/
              __asm { vcvttss2si eax, dword ptr [esi+0A8h] } /*0x10032da8*/
              i = _EAX; /*0x10032db0*/
              __asm { vcvttss2si ebx, dword ptr [esi+7Ch] } /*0x10032db9*/
              v1980 = v1727; /*0x10032dc1*/
              __asm { vcvttss2si edx, dword ptr [esi+90h] } /*0x10032dc7*/
              sub_101778E0(a1: _EDX, a2: _EBX, a3: _EAX, a4: v1730, a5: v1727, a6: 1); /*0x10032dda*/
            }
            else if ( dword_103F4DEC == 0 ) /*0x100331e6*/
            {
              __asm /*0x100331ec*/
              {
                vmovss xmm0, dword ptr [esi+34h]
                vmovss dword ptr [esi+18h], xmm0
                vcvttss2si eax, xmm0
              }
              v1729 = _EAX; /*0x100331fa*/
              __asm /*0x100331fd*/
              {
                vmovss xmm0, dword ptr [esi+0A8h]
                vmovss dword ptr [esi+14h], xmm0
                vcvttss2si eax, xmm0
              }
              v1728 = _EAX; /*0x1003320e*/
              __asm /*0x10033217*/
              {
                vmovss xmm0, dword ptr [esi+7Ch]
                vmovss dword ptr [esi+28h], xmm0
                vcvttss2si ebx, xmm0
              }
              LOBYTE(_EAX) = v1727; /*0x10033225*/
              v1981 = v1727; /*0x10033228*/
              __asm /*0x1003322e*/
              {
                vmovss xmm0, dword ptr [esi+90h]
                vmovss dword ptr [esi+60h], xmm0
                vcvttss2si edx, xmm0
              }
              sub_10177840(a1: _EDX, a2: _EBX, a3: v1728, a4: v1729, a5: (char)_EAX, a6: 1); /*0x1003324a*/
              __asm /*0x1003324f*/
              {
                vmovss xmm1, ds:dword_10185854
                vaddss xmm0, xmm1, dword ptr [esi+18h]
                vcvttss2si eax, xmm0
              }
              v1729 = _EAX; /*0x10033260*/
              __asm /*0x10033263*/
              {
                vaddss xmm0, xmm1, dword ptr [esi+14h]
                vcvttss2si eax, xmm0
              }
              v1728 = _EAX; /*0x1003326c*/
              __asm /*0x10033275*/
              {
                vmovss xmm1, ds:dword_1018555C
                vaddss xmm0, xmm1, dword ptr [esi+28h]
                vcvttss2si ebx, xmm0
              }
              v1930 = -16777216; /*0x10033286*/
              __asm /*0x10033290*/
              {
                vaddss xmm0, xmm1, dword ptr [esi+60h]
                vcvttss2si edx, xmm0
              }
              sub_10177840(a1: _EDX, a2: _EBX, a3: _EAX, a4: v1729, a5: 0, a6: 1); /*0x100332aa*/
              __asm /*0x100332af*/
              {
                vmovss xmm1, ds:dword_10185850
                vaddss xmm0, xmm1, dword ptr [esi+18h]
                vcvttss2si eax, xmm0
              }
              i = _EAX; /*0x100332c0*/
              __asm /*0x100332c3*/
              {
                vaddss xmm0, xmm1, dword ptr [esi+14h]
                vcvttss2si eax, xmm0
              }
              v1725 = _EAX; /*0x100332cc*/
              __asm /*0x100332d5*/
              {
                vmovss xmm1, ds:dword_10184184
                vaddss xmm0, xmm1, dword ptr [esi+28h]
                vcvttss2si ebx, xmm0
              }
              v1929 = -16777216; /*0x100332e6*/
              __asm /*0x100332f0*/
              {
                vaddss xmm0, xmm1, dword ptr [esi+60h]
                vcvttss2si edx, xmm0
              }
              sub_10177840(a1: _EDX, a2: _EBX, a3: _EAX, a4: i, a5: 0, a6: 1); /*0x1003330a*/
            }
          }
          goto LABEL_365; /*0x1003330f*/
        }
LABEL_380:
        i = *(char ***)ArgList; /*0x10033139*/
        v1932 = v1727; /*0x10033144*/
        __asm /*0x1003314a*/
        {
          vmovss xmm0, ds:dword_10184EDC
          vmulss xmm0, xmm0, dword ptr [esi+34h]
          vaddss xmm0, xmm0, dword ptr [esi+7Ch]
        }
        __asm
        {
          vcvtsi2ss xmm1, xmm3, eax
          vsubss xmm0, xmm0, xmm1
          vcvttss2si eax, xmm0
        }
        v1730 = _EAX; /*0x10033174*/
        __asm /*0x10033177*/
        {
          vmovss xmm0, ds:dword_10185884
          vaddss xmm0, xmm0, dword ptr [esi+90h]
          vaddss xmm0, xmm0, ds:dword_1018550C
          vcvttss2si ebx, xmm0
        }
        sub_1017B960(a1: &v1897); /*0x100331a0*/
        sub_10177250( /*0x100331c1*/
          a1: (int)i,
          a2: (int)&v1897,
          a3: _EBX,
          a4: (int)v1730,
          a5: v1932,
          a6: 1,
          Format: (char *)"Ammunition",
          ArgList: v1724[0]);
        goto LABEL_381; /*0x100331c1*/
      }
LABEL_379:
      __asm /*0x100330af*/
      {
        vmovss xmm0, dword ptr [esi+7Ch]
        vaddss xmm0, xmm0, dword ptr [esi+34h]
      }
      i = *(char ***)ArgList; /*0x100330be*/
      __asm /*0x100330c1*/
      {
        vaddss xmm0, xmm0, ds:dword_10185854
        vcvttss2si eax, xmm0
      }
      v1725 = _EAX; /*0x100330cd*/
      __asm /*0x100330d0*/
      {
        vmovss xmm0, ds:dword_10184EDC
        vmulss xmm0, xmm0, dword ptr [esi+0A8h]
        vaddss xmm0, xmm0, dword ptr [esi+90h]
      }
      v1933 = v1727; /*0x100330eb*/
      __asm { vcvttss2si ebx, xmm0 } /*0x100330f1*/
      sub_1017B960(a1: &v1897); /*0x10033102*/
      sub_10177250( /*0x10033123*/
        a1: (int)i,
        a2: (int)&v1897,
        a3: _EBX,
        a4: (int)v1725,
        a5: v1933,
        a6: 8,
        Format: (char *)"Ammunition",
        ArgList: v1724[0]);
      if ( dword_103F4E4C != 3 ) /*0x10033133*/
        goto LABEL_381; /*0x10033133*/
      goto LABEL_380; /*0x10033133*/
    }
  }
LABEL_360:
  if ( (_BYTE)word_103F4BDD == 1 ) /*0x10032cf8*/
  {
    v2004 = -1; /*0x10032d04*/
    result = sub_10179540(); /*0x10032d0e*/
    if ( result == 0 ) /*0x10032d15*/
      return result; /*0x10032d15*/
    v995 = result; /*0x10032d1b*/
    __asm /*0x10032d1d*/
    {
      vmovss xmm0, ds:dword_10184380
      vmulss xmm1, xmm0, dword ptr qword_103F4C74
      vmovss dword ptr [esi+40h], xmm1
      vmulss xmm1, xmm0, dword ptr qword_103F4C74+4
      vmovss dword ptr [esi+14h], xmm1
      vmulss xmm0, xmm0, dword_103F4C7C
      vmovss dword ptr [esi+28h], xmm0
    }
    v1000 = (char ***)sub_10029F30(a1: 0); /*0x10032d54*/
    v1001 = *v1000; /*0x10032d59*/
    i = v1000[1]; /*0x10032d5e*/
    if ( v1001 != i ) /*0x10032d63*/
    {
      __asm { vcvttss2si eax, dword ptr [esi+40h] } /*0x10032d69*/
      v1733 = _EAX; /*0x10032d6e*/
      __asm { vcvttss2si eax, dword ptr [esi+14h] } /*0x10032d71*/
      v1754 = _EAX; /*0x10032d76*/
      __asm { vcvttss2si eax, dword ptr [esi+28h] } /*0x10032d7c*/
      v1735 = _EAX; /*0x10032d81*/
      __asm /*0x10032d84*/
      {
        vbroadcastss xmm0, ds:dword_10184EE0
        vmovaps xmmword ptr [esi+260h], xmm0
      }
      v1743 = (char **)v995; /*0x10032d95*/
      do /*0x1003332f*/
      {
        if ( *v1001 == nullptr || *v1001 == (char *)v995 ) /*0x10033338*/
          goto LABEL_386; /*0x1003333d*/
        v1730 = *v1001; /*0x1003333f*/
        if ( dword_103EEA68 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x1003335d*/
        {
          sub_101809C3(a1: &dword_103EEA68); /*0x100339cd*/
          if ( dword_103EEA68 == -1 ) /*0x100339dc*/
          {
            v1734 = (wchar_t *)v1001; /*0x100339e2*/
            v2004 = 11; /*0x100339e5*/
            dword_103EEA64 = sub_1017C730(a1: "CBasePlayer", a2: "deadflag"); /*0x10033a01*/
            sub_10180A43(a1: &dword_103EEA68); /*0x10033a0b*/
            v995 = (int)v1743; /*0x10033a13*/
            v1001 = (char **)v1734; /*0x10033a19*/
          }
        }
        if ( *(_DWORD *)&v1730[dword_103EEA64] != 0 ) /*0x1003336f*/
          goto LABEL_386; /*0x1003336f*/
        v1103 = *(int (**)(void))(*(_DWORD *)v1730 + 960); /*0x10033373*/
        v2004 = -1; /*0x10033379*/
        v1104 = v1730; /*0x10033383*/
        LOBYTE(v1734) = v1103(); /*0x10033387*/
        _EAX = (*(int (__thiscall **)(char *))(*(_DWORD *)v1104 + 44))(a1: v1104); /*0x1003338e*/
        __asm /*0x10033391*/
        {
          vmovss xmm2, ds:dword_10184380
          vmulss xmm0, xmm2, dword ptr qword_103F4F8C
          vcvttss2si ecx, xmm0
        }
        v1725 = _ECX; /*0x100333a5*/
        __asm /*0x100333a8*/
        {
          vmulss xmm0, xmm2, dword ptr qword_103F4F8C+4
          vcvttss2si edx, xmm0
          vmulss xmm0, xmm2, dword_103F4F94
          vcvttss2si ebx, xmm0
          vmulss xmm0, xmm2, dword ptr qword_103F4FA4
          vmulss xmm1, xmm2, dword ptr qword_103F4FA4+4
          vmulss xmm2, xmm2, dword_103F4FAC
          vcvttss2si ecx, xmm0
        }
        v1729 = _ECX; /*0x100333dc*/
        __asm { vcvttss2si ecx, xmm1 } /*0x100333df*/
        v1728 = _ECX; /*0x100333e3*/
        __asm { vcvttss2si ecx, xmm2 } /*0x100333e6*/
        v1731 = _ECX; /*0x100333ea*/
        if ( (_BYTE)v1734 != 0 ) /*0x100333f1*/
        {
          v1119 = v1725; /*0x100333fa*/
          if ( byte_103F4BDC != 0 ) /*0x100333fd*/
          {
            v1729 = (int)v1733; /*0x10033402*/
            _EDX = v1754; /*0x10033405*/
            v1728 = v1754; /*0x1003340d*/
            v1731 = v1735; /*0x10033413*/
            v1119 = v1733; /*0x10033416*/
            _EBX = v1735; /*0x10033419*/
          }
        }
        else
        {
          v1119 = v1725; /*0x1003341e*/
        }
        v1752 = (char *)_EBX; /*0x10033421*/
        v1753 = _EDX; /*0x10033427*/
        v1725 = v1119; /*0x1003342d*/
        v1734 = (wchar_t *)v1001; /*0x10033430*/
        __asm /*0x10033433*/
        {
          vmovss xmm0, dword ptr [eax]
          vmovss dword ptr [esi+0D8h], xmm0
          vmovss xmm0, dword ptr [eax+4]
          vmovss dword ptr [esi+0E8h], xmm0
          vmovss xmm0, dword ptr [eax+8]
          vmovss dword ptr [esi+0E4h], xmm0
        }
        v1123 = v1730; /*0x10033459*/
        (*(void (__thiscall **)(char *))(*(_DWORD *)v1730 + 624))(a1: v1730); /*0x10033460*/
        __asm /*0x10033468*/
        {
          vmovss xmm1, ds:dword_10184F10
          vmulss xmm0, xmm1, dword ptr [eax]
        }
        v1897 = (float *)v1732; /*0x10033477*/
        v1744[0] = &v1739; /*0x10033480*/
        __asm { vmovss dword ptr [esi+90h], xmm0 } /*0x10033486*/
        v1126 = sin(v1741[0]); /*0x10033494*/
        v1127 = (float *)v1744[0]; /*0x1003349c*/
        *v1897 = cos(v1741[0]); /*0x100334a2*/
        *v1127 = v1126; /*0x100334a4*/
        __asm { vmulss xmm0, xmm1, dword ptr [ecx+4] } /*0x100334a6*/
        v1897 = (float *)&v1736; /*0x100334ae*/
        v1744[0] = v1742; /*0x100334ba*/
        __asm { vmovss dword ptr [esi+90h], xmm0 } /*0x100334c0*/
        v1129 = sin(v1741[0]); /*0x100334ce*/
        v1130 = (float *)v1744[0]; /*0x100334d6*/
        *v1897 = cos(v1741[0]); /*0x100334dc*/
        *v1130 = v1129; /*0x100334de*/
        __asm /*0x100334e0*/
        {
          vmovss xmm0, dword ptr [esi+7Ch]
          vmovss dword ptr [esi+1Ch], xmm0
          vmovss xmm0, dword ptr [esi+58h]
          vinsertps xmm0, xmm0, dword ptr [esi+0A8h], 10h
          vbroadcastss xmm1, dword ptr [esi+34h]
          vmulps xmm0, xmm1, xmm0
          vbroadcastss xmm1, ds:dword_10185890
          vmulps xmm0, xmm0, xmm1
          vmovaps xmmword ptr [esi+60h], xmm0
        }
        (*(void (__thiscall **)(char *, _DWORD *))(*(_DWORD *)v1123 + 620))(a1: v1123, a2: v1744); /*0x10033520*/
        __asm /*0x10033526*/
        {
          vmovss xmm0, dword ptr [esi+1Ch]
          vmulss xmm0, xmm0, ds:dword_10185890
          vmovss xmm1, dword ptr [esi+0C8h]
          vsubss xmm0, xmm1, xmm0
          vmovsd xmm1, qword ptr [esi+0C0h]
          vaddps xmm1, xmm1, xmmword ptr [esi+60h]
          vmovlps qword ptr [esi+90h], xmm1
          vmovss dword ptr [esi+98h], xmm0
        }
        if ( dword_103EEAD8 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x10033577*/
        {
          sub_101809C3(a1: &dword_103EEAD8); /*0x10033a26*/
          if ( dword_103EEAD8 == -1 ) /*0x10033a35*/
          {
            v2004 = 12; /*0x10033a3b*/
            dword_103EEAD4 = sub_1017C730(a1: "CBaseEntity", a2: "m_iTeamNum"); /*0x10033a57*/
            sub_10180A43(a1: &dword_103EEAD8); /*0x10033a61*/
            v1123 = v1730; /*0x10033a6f*/
          }
        }
        v1727 = *(_DWORD *)&v1123[dword_103EEAD4]; /*0x10033585*/
        v1732[0] = &CTraceFilterHitscan::`vftable'; /*0x10033588*/
        v1732[1] = v1123; /*0x1003358f*/
        __asm /*0x10033592*/
        {
          vxorps xmm0, xmm0, xmm0
          vmovups ymmword ptr [esi+1F4h], ymm0
          vmovaps ymmword ptr [esi+1E0h], ymm0
          vmovaps ymmword ptr [esi+1C0h], ymm0
        }
        v2004 = -1; /*0x100335b4*/
        __asm { vzeroupper } /*0x100335d6*/
        sub_100277C0(a1: v1744, a2: v1741, a3: 33570827, a4: v1732, a5: &v1897); /*0x100335d9*/
        v1145 = v1727; /*0x100335de*/
        __asm /*0x100335e1*/
        {
          vmovsd xmm0, qword ptr [esi+1CCh]
          vmovsd qword ptr [esi+90h], xmm0
          vmovss xmm0, dword ptr [esi+1D4h]
          vmovss dword ptr [esi+98h], xmm0
        }
        if ( v1145 == 2 ) /*0x10033604*/
        {
          v1001 = (char **)v1734; /*0x10033804*/
          v995 = (int)v1743; /*0x10033807*/
          if ( byte_103F4FA0 != 1 || byte_103F4BE5 != 1 || byte_103F4D18 != 1 ) /*0x10033827*/
            goto LABEL_386; /*0x10033827*/
          sub_1002DDD0(a1: &v1739, a2: v1730, a3: v1743); /*0x10033835*/
          __asm /*0x1003383d*/
          {
            vmovss xmm0, dword ptr [esi+7Ch]
            vandps xmm0, xmm0, xmmword ptr [esi+260h]
            vmovss xmm1, ds:dword_10184EE4
            vucomiss xmm1, xmm0
          }
          if ( (unsigned int)&v1723[8] < 0xFFFFFFF4 && v1724 != nullptr ) /*0x1003383a*/
          {
            __asm /*0x10033858*/
            {
              vmovss xmm0, dword ptr [esi+80h]
              vandps xmm0, xmm0, xmmword ptr [esi+260h]
              vucomiss xmm1, xmm0
            }
            __asm
            {
              vmovss xmm0, dword ptr [esi+84h]
              vandps xmm0, xmm0, xmmword ptr [esi+260h]
              vucomiss xmm1, xmm0
            }
            goto LABEL_386; /*0x10033882*/
          }
          __asm /*0x10033888*/
          {
            vxorps xmm0, xmm0, xmm0
            vmovsd qword ptr [esi+0A8h], xmm0
          }
          v1742[2] = 0; /*0x10033894*/
          if ( (unsigned __int8)sub_10177080(a1: &v1739, a2: v1742) == 0 ) /*0x100338b6*/
            goto LABEL_386; /*0x100338b6*/
          (*(void (__thiscall **)(int))(*(_DWORD *)v995 + 44))(a1: v995); /*0x100338c0*/
          __asm /*0x100338c3*/
          {
            vmovss xmm0, dword ptr [esi+0E4h]
            vsubss xmm0, xmm0, dword ptr [eax+8]
            vmovss xmm1, dword ptr [esi+0E8h]
            vsubss xmm1, xmm1, dword ptr [eax+4]
            vmovss xmm2, dword ptr [esi+0D8h]
            vsubss xmm2, xmm2, dword ptr [eax]
            vmulss xmm1, xmm1, xmm1
            vfmadd231ss xmm1, xmm2, xmm2
            vfmadd231ss xmm1, xmm0, xmm0
            vcvtss2sd xmm0, xmm1, xmm1
            vsqrtsd xmm0, xmm0, xmm0
            vcvtsd2ss xmm0, xmm0, xmm0
            vucomiss xmm0, dword_103F4BE0
          }
          if ( !(v89 | v90) ) /*0x1003390b*/
            goto LABEL_386; /*0x1003390b*/
          v1197 = dword_103F4FB4; /*0x10033911*/
          if ( dword_103F4FB4 == 1 ) /*0x10033919*/
          {
            v1764 = v1729; /*0x10033924*/
            v1765 = (char)v1728; /*0x1003392d*/
            v1766 = v1731; /*0x10033936*/
            __asm { vcvttss2si eax, dword ptr [esi+0ACh] } /*0x1003393c*/
            v1767 = -1; /*0x10033944*/
            __asm { vcvttss2si edx, dword ptr [esi+0A8h] } /*0x1003394b*/
            v1001 = (char **)v1734; /*0x1003395a*/
            sub_10178BD0(a1: _EDX, a2: _EAX, a3: 5, a4: 24, a5: v1729); /*0x10033963*/
            v1197 = dword_103F4FB4; /*0x10033968*/
          }
          if ( v1197 != 0 ) /*0x1003396f*/
            goto LABEL_386; /*0x1003396f*/
          LOBYTE(v1763) = v1729; /*0x1003397e*/
          BYTE1(v1763) = (_BYTE)v1728; /*0x10033987*/
          BYTE2(v1763) = v1731; /*0x10033990*/
          HIBYTE(v1763) = -1; /*0x10033996*/
          __asm /*0x1003399d*/
          {
            vmovss xmm1, ds:dword_10185884
            vaddss xmm0, xmm1, dword ptr [esi+0ACh]
            vcvttss2si eax, xmm0
            vaddss xmm0, xmm1, dword ptr [esi+0A8h]
            vcvttss2si edx, xmm0
          }
          v1176 = v1763; /*0x100339bd*/
        }
        else
        {
          v1001 = (char **)v1734; /*0x1003360d*/
          v995 = (int)v1743; /*0x10033610*/
          if ( v1145 != 3 || byte_103F4F88 != 1 || byte_103F4BE4 != 1 || byte_103F4C22 != 1 ) /*0x1003363d*/
            goto LABEL_386; /*0x1003363d*/
          sub_1002DDD0(a1: &v1739, a2: v1730, a3: v1743); /*0x1003364b*/
          __asm /*0x10033653*/
          {
            vmovss xmm0, dword ptr [esi+7Ch]
            vandps xmm0, xmm0, xmmword ptr [esi+260h]
            vmovss xmm1, ds:dword_10184EE4
            vucomiss xmm1, xmm0
          }
          if ( (unsigned int)&v1723[8] < 0xFFFFFFF4 && v1724 != nullptr ) /*0x10033650*/
          {
            __asm /*0x1003366e*/
            {
              vmovss xmm0, dword ptr [esi+80h]
              vandps xmm0, xmm0, xmmword ptr [esi+260h]
              vucomiss xmm1, xmm0
            }
            __asm
            {
              vmovss xmm0, dword ptr [esi+84h]
              vandps xmm0, xmm0, xmmword ptr [esi+260h]
              vucomiss xmm1, xmm0
            }
            goto LABEL_386; /*0x10033698*/
          }
          (*(void (__thiscall **)(int))(*(_DWORD *)v995 + 44))(a1: v995); /*0x100336a2*/
          __asm /*0x100336a5*/
          {
            vmovss xmm0, dword ptr [esi+0E4h]
            vsubss xmm0, xmm0, dword ptr [eax+8]
            vmovss xmm1, dword ptr [esi+0E8h]
            vsubss xmm1, xmm1, dword ptr [eax+4]
            vmovss xmm2, dword ptr [esi+0D8h]
            vsubss xmm2, xmm2, dword ptr [eax]
            vmulss xmm1, xmm1, xmm1
            vfmadd231ss xmm1, xmm2, xmm2
            vfmadd231ss xmm1, xmm0, xmm0
            vcvtss2sd xmm0, xmm1, xmm1
            vsqrtsd xmm0, xmm0, xmm0
            vcvtsd2ss xmm0, xmm0, xmm0
            vucomiss xmm0, dword_103F4BE0
          }
          if ( !(v89 | v90) ) /*0x100336ed*/
            goto LABEL_386; /*0x100336ed*/
          __asm /*0x100336f3*/
          {
            vxorps xmm0, xmm0, xmm0
            vmovsd qword ptr [esi+0A8h], xmm0
          }
          v1742[2] = 0; /*0x100336ff*/
          if ( (unsigned __int8)sub_10177080(a1: &v1739, a2: v1742) == 0 ) /*0x10033721*/
            goto LABEL_386; /*0x10033721*/
          sub_101669E0(a1: &v1736, a2: v995, a3: v1730); /*0x10033735*/
          v1168 = dword_103F4F9C; /*0x1003373a*/
          if ( dword_103F4F9C == 1 ) /*0x10033742*/
          {
            v1769 = (char)v1725; /*0x1003374d*/
            v1770 = (char)v1753; /*0x10033759*/
            v1771 = (char)v1752; /*0x10033765*/
            __asm { vcvttss2si eax, dword ptr [esi+0ACh] } /*0x1003376b*/
            v1772 = -1; /*0x10033773*/
            __asm { vcvttss2si edx, dword ptr [esi+0A8h] } /*0x1003377a*/
            v1001 = (char **)v1734; /*0x10033789*/
            sub_10178BD0(a1: _EDX, a2: _EAX, a3: 5, a4: 24, a5: (char)v1725); /*0x10033792*/
            v1168 = dword_103F4F9C; /*0x10033797*/
          }
          if ( v1168 != 0 ) /*0x1003379e*/
            goto LABEL_386; /*0x1003379e*/
          LOBYTE(v1768) = (_BYTE)v1725; /*0x100337ad*/
          BYTE1(v1768) = (_BYTE)v1753; /*0x100337b9*/
          BYTE2(v1768) = (_BYTE)v1752; /*0x100337c5*/
          HIBYTE(v1768) = -1; /*0x100337cb*/
          __asm /*0x100337d2*/
          {
            vmovss xmm1, ds:dword_10185884
            vaddss xmm0, xmm1, dword ptr [esi+0ACh]
            vcvttss2si eax, xmm0
            vaddss xmm0, xmm1, dword ptr [esi+0A8h]
            vcvttss2si edx, xmm0
          }
          v1176 = v1768; /*0x100337f2*/
        }
        *(_DWORD *)&v1723[16] = v1176; /*0x10033314*/
        v1001 = (char **)v1734; /*0x10033315*/
        sub_10177780(a1: _EDX, a2: _EAX, a3: 8, a4: 8, a5: *(_DWORD *)&v1723[16]); /*0x1003331e*/
LABEL_386:
        ++v1001; /*0x10033323*/
      }
      while ( v1001 != i ); /*0x1003332f*/
    }
  }
  if ( (_BYTE)word_103F4BDD == 1 ) /*0x10033a7e*/
  {
    v2004 = -1; /*0x10033a86*/
    result = sub_10179540(); /*0x10033a90*/
    if ( result == 0 ) /*0x10033a97*/
      return result; /*0x10033a97*/
    v1203 = (char *)result; /*0x10033a9d*/
    __asm /*0x10033a9f*/
    {
      vmovss xmm0, dword ptr qword_103F4C74
      vmovss dword ptr [esi+30h], xmm0
      vmovss xmm0, dword ptr qword_103F4C74+4
      vmovss dword ptr [esi+0BCh], xmm0
      vmovss xmm0, dword_103F4C7C
      vmovss dword ptr [esi+3Ch], xmm0
    }
    v1207 = (char ***)sub_10029F30(a1: 0); /*0x10033ad1*/
    v1208 = *v1207; /*0x10033ad6*/
    for ( i = v1207[1]; v1208 != i; ++v1208 ) /*0x10033ae0*/
    {
      v1661 = *v1208; /*0x1003568e*/
      if ( *v1208 != nullptr ) /*0x10035692*/
      {
        v1662 = *(int (**)(void))(*(_DWORD *)v1661 + 44); /*0x10035696*/
        v2004 = -1; /*0x10035699*/
        v1730 = v1661; /*0x100356a3*/
        _EAX = v1662(); /*0x100356a9*/
        if ( v1730 != v1203 ) /*0x100356ae*/
        {
          __asm /*0x100356b0*/
          {
            vmovss xmm0, dword ptr [eax]
            vmovss xmm1, dword ptr [eax+4]
            vmovss xmm2, dword ptr [eax+8]
          }
          if ( dword_103EEA68 > *(_DWORD *)(*((_DWORD *)NtCurrentTeb()->ThreadLocalStoragePointer + TlsIndex) + 4) ) /*0x100356d9*/
          {
            *(_DWORD *)&v1723[16] = &dword_103EEA68; /*0x10035a60*/
            __asm /*0x10035a65*/
            {
              vmovss dword ptr [esi+14h], xmm0
              vmovss dword ptr [esi+40h], xmm1
              vmovss dword ptr [esi+1Ch], xmm2
            }
            sub_101809C3(a1: *(_DWORD *)&v1723[16]); /*0x10035a74*/
            __asm /*0x10035a79*/
            {
              vmovss xmm2, dword ptr [esi+1Ch]
              vmovss xmm1, dword ptr [esi+40h]
              vmovss xmm0, dword ptr [esi+14h]
            }
            if ( dword_103EEA68 == -1 ) /*0x10035a92*/
            {
              v1746 = v1208; /*0x10035a98*/
              v1735 = (int)v1203; /*0x10035a9e*/
              v2004 = 13; /*0x10035aa1*/
              dword_103EEA64 = sub_1017C730(a1: "CBasePlayer", a2: "deadflag"); /*0x10035abd*/
              sub_10180A43(a1: &dword_103EEA68); /*0x10035ac7*/
              v1203 = (char *)v1735; /*0x10035acf*/
              v1208 = v1746; /*0x10035ad2*/
              __asm /*0x10035ad8*/
              {
                vmovss xmm0, dword ptr [esi+14h]
                vmovss xmm1, dword ptr [esi+40h]
                vmovss xmm2, dword ptr [esi+1Ch]
              }
            }
          }
          if ( *(_DWORD *)&v1730[dword_103EEA64] == 0 ) /*0x100356eb*/
          {
            v1667 = *(void (__thiscall **)(char *))(*(_DWORD *)v1203 + 44); /*0x100356ef*/
            v2004 = -1; /*0x100356f2*/
            __asm /*0x100356fe*/
            {
              vmovss dword ptr [esi+14h], xmm0
              vmovss dword ptr [esi+40h], xmm1
              vmovss dword ptr [esi+1Ch], xmm2
            }
            v1667(a1: v1203); /*0x1003570d*/
            __asm /*0x1003570f*/
            {
              vmovss xmm0, dword ptr [esi+1Ch]
              vsubss xmm0, xmm0, dword ptr [eax+8]
              vmovss xmm1, dword ptr [esi+40h]
              vsubss xmm1, xmm1, dword ptr [eax+4]
              vmovss xmm2, dword ptr [esi+14h]
              vsubss xmm2, xmm2, dword ptr [eax]
              vmulss xmm1, xmm1, xmm1
              vfmadd231ss xmm1, xmm2, xmm2
              vfmadd231ss xmm1, xmm0, xmm0
              vcvtss2sd xmm0, xmm1, xmm1
              vsqrtsd xmm0, xmm0, xmm0
              vcvtsd2ss xmm0, xmm0, xmm0
              vucomiss xmm0, dword_103F4BE0
            }
            if ( v89 | v90 ) /*0x1003574e*/
            {
              if ( *(_DWORD *)sub_10029FF0(a1: v1730) != 2 && *(_DWORD *)sub_10035DA0(a1: v1730) == 2 ) /*0x10035773*/
              {
                v1746 = v1208; /*0x10035779*/
                v1735 = (int)v1203; /*0x1003577f*/
                v1680 = v1730; /*0x10035784*/
                _EAX = (*(int (**)(void))(*(_DWORD *)v1730 + 44))(); /*0x10035786*/
                __asm /*0x10035789*/
                {
                  vmovsd xmm0, qword ptr [eax]
                  vmovaps xmmword ptr [esi+40h], xmm0
                  vmovss xmm0, dword ptr [eax+8]
                  vmovss dword ptr [esi+1Ch], xmm0
                }
                LOBYTE(v1730) = (*(int (__thiscall **)(char *))(*(_DWORD *)v1680 + 960))(a1: v1680); /*0x100357a6*/
                __asm /*0x100357a9*/
                {
                  vmovss xmm0, dword_103F4FBC
                  vmovss dword ptr [esi+14h], xmm0
                  vmovss xmm0, dword_103F4FC0
                  vmovss dword ptr [esi+60h], xmm0
                  vmovss xmm0, dword_103F4FC4
                  vmovss dword ptr [esi+24h], xmm0
                }
                v1897 = nullptr; /*0x100357d0*/
                v1898 = nullptr; /*0x100357da*/
                v1899[0] = 0; /*0x100357e4*/
                v1744[0] = 60; /*0x100357ee*/
                v2004 = 14; /*0x100357f8*/
                sub_1003CA10(a1: v1744); /*0x1003580f*/
                __asm /*0x10035814*/
                {
                  vmovss xmm0, dword ptr [esi+30h]
                  vmovss xmm1, dword ptr [esi+0BCh]
                  vmovss xmm2, dword ptr [esi+3Ch]
                }
                if ( ((unsigned __int8)v1730 & (unsigned __int8)byte_103F4BDC) == 0 ) /*0x10035830*/
                {
                  __asm /*0x10035832*/
                  {
                    vmovss xmm0, dword ptr [esi+14h]
                    vmovss xmm1, dword ptr [esi+60h]
                    vmovss xmm2, dword ptr [esi+24h]
                  }
                }
                if ( byte_103F4FB8 == 1 && byte_103F4BE4 == 1 && byte_103F4C22 == 1 ) /*0x10035862*/
                {
                  __asm /*0x10035868*/
                  {
                    vmovss xmm3, ds:dword_10184380
                    vmulss xmm2, xmm2, xmm3
                    vcvttss2si eax, xmm2
                  }
                  v1738 = _EAX; /*0x10035878*/
                  __asm /*0x1003587b*/
                  {
                    vmulss xmm1, xmm1, xmm3
                    vcvttss2si eax, xmm1
                  }
                  v1729 = _EAX; /*0x10035883*/
                  __asm /*0x10035886*/
                  {
                    vmulss xmm0, xmm0, xmm3
                    vcvttss2si eax, xmm0
                  }
                  v1728 = _EAX; /*0x1003588e*/
                  for ( j = nullptr; (unsigned int)j < 0x3C; j = v1725 + 1 ) /*0x10035891*/
                  {
                    v1725 = j; /*0x100358a2*/
                    __asm /*0x100358a5*/
                    {
                      vcvtsi2sd xmm0, xmm4, eax
                      vmulsd xmm0, xmm0, ds:qword_10185898
                      vdivsd xmm0, xmm0, ds:qword_101858A0
                      vcvtsd2ss xmm0, xmm0, xmm0
                      vcvtss2sd xmm0, xmm0, xmm0
                      vmovsd qword ptr [esi+28h], xmm0
                    }
                    __asm { vmovsd qword ptr [esp], xmm0 }
                    v1961 = cos(X: *(double *)&v1723[12]); /*0x100358d6*/
                    __asm /*0x100358df*/
                    {
                      vmovsd xmm0, qword ptr [esi+28h]
                      vmovsd qword ptr [esp], xmm0
                    }
                    v1960 = sin(X: *(double *)&v1723[12]); /*0x100358f1*/
                    __asm /*0x100358f7*/
                    {
                      vmovsd xmm0, qword ptr [esi+378h]
                      vmovhpd xmm0, xmm0, qword ptr [esi+370h]
                      vcvtpd2ps xmm0, xmm0
                      vbroadcastss xmm1, ds:dword_101858A8
                      vfmadd213ps xmm0, xmm1, xmmword ptr [esi+40h]
                      vmovlps qword ptr [esi+0C0h], xmm0
                      vmovss xmm0, dword ptr [esi+1Ch]
                      vmovss dword ptr [esi+0C8h], xmm0
                      vxorps xmm0, xmm0, xmm0
                      vmovsd qword ptr [esi+90h], xmm0
                    }
                    v1741[2] = 0.0; /*0x1003593b*/
                    if ( (unsigned __int8)sub_10177080(a1: v1744, a2: v1741) != 0 ) /*0x10035960*/
                      sub_10035F40(a1: v1741); /*0x10035973*/
                  }
                  for ( k = 1; ; k = (unsigned int)(v1725 + 1) ) /*0x1003597d*/
                  {
                    _EAX = v1897; /*0x10035982*/
                    _ECX = v1898; /*0x10035988*/
                    if ( k >= -1431655765 * ((v1898 - (char *)v1897) >> 2) ) /*0x1003599d*/
                      break; /*0x1003599d*/
                    LOBYTE(v1762) = (_BYTE)v1728; /*0x100359a2*/
                    BYTE1(v1762) = v1729; /*0x100359ab*/
                    BYTE2(v1762) = (_BYTE)v1738; /*0x100359b4*/
                    HIBYTE(v1762) = -1; /*0x100359ba*/
                    v1725 = (char *)k; /*0x100359c1*/
                    _ECX = 3 * k; /*0x100359c4*/
                    __asm /*0x100359c7*/
                    {
                      vcvttss2si edx, dword ptr [eax+ecx*4+4]
                      vcvttss2si edi, dword ptr [eax+ecx*4]
                      vcvttss2si ebx, dword ptr [eax+ecx*4-8]
                      vcvttss2si eax, dword ptr [eax+ecx*4-0Ch]
                    }
                    v1730 = _EAX; /*0x100359de*/
                    sub_10177600(a1: _EAX, a2: _EBX, a3: _EDI, a4: _EDX, a5: v1762, a6: 1); /*0x100359f6*/
                  }
                  if ( v1897 != (float *)v1898 ) /*0x10035a03*/
                  {
                    LOBYTE(v1761) = (_BYTE)v1728; /*0x10035a0c*/
                    BYTE1(v1761) = v1729; /*0x10035a15*/
                    BYTE2(v1761) = (_BYTE)v1738; /*0x10035a1e*/
                    HIBYTE(v1761) = -1; /*0x10035a24*/
                    __asm /*0x10035a2b*/
                    {
                      vcvttss2si edx, dword ptr [eax+4]
                      vcvttss2si eax, dword ptr [eax]
                    }
                    v1730 = _EAX; /*0x10035a34*/
                    __asm /*0x10035a37*/
                    {
                      vcvttss2si edi, dword ptr [ecx-8]
                      vcvttss2si ebx, dword ptr [ecx-0Ch]
                    }
                    sub_10177600(a1: _EBX, a2: _EDI, a3: _EAX, a4: _EDX, a5: v1761, a6: 1); /*0x10035a56*/
                  }
                }
                sub_10015AD0(a1: &v1897); /*0x10035674*/
                v1203 = (char *)v1735; /*0x10035679*/
                v1208 = v1746; /*0x1003567c*/
              }
            }
          }
        }
      }
    }
  }
  if ( (_BYTE)word_103F4BDD == 1 ) /*0x10033aed*/
  {
    v2004 = -1; /*0x10033afa*/
    if ( byte_103F4DE4 == 1 ) /*0x10033b06*/
    {
      (*(void (__thiscall **)(int, int))(*(_DWORD *)dword_103EC728 + 420))(a1: dword_103EC728, a2: 1065353216); /*0x10033b19*/
      if ( (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC7B8 + 32))(a1: dword_103EC7B8) > 0 ) /*0x10033b2c*/
      {
        v1209 = nullptr; /*0x10033b32*/
        while ( 1 ) /*0x10033b62*/
        {
          v1211 = *(_DWORD *)dword_103EC7B8; /*0x10033b62*/
          v1730 = v1209 + 1; /*0x10033b64*/
          v1212 = (*(int (__thiscall **)(int, char *))(v1211 + 12))(a1: dword_103EC7B8, a2: v1209 + 1); /*0x10033b68*/
          if ( v1212 != 0 ) /*0x10033b6d*/
          {
            v1213 = (_DWORD *)v1212; /*0x10033b6f*/
            v1214 = v1212 + 8; /*0x10033b73*/
            if ( (*(unsigned __int8 (__thiscall **)(int))(*(_DWORD *)(v1212 + 8) + 28))(a1: v1212 + 8) == 0 ) /*0x10033b80*/
            {
              (*(void (__thiscall **)(_DWORD *))(*v1213 + 44))(a1: v1213); /*0x10033b86*/
              v1744[0] = 0; /*0x10033b89*/
              v1741[0] = 0.0; /*0x10033b93*/
              v1739 = 0; /*0x10033b9d*/
              v1742[0] = 0; /*0x10033ba4*/
              v1215 = (*(int (__thiscall **)(int))(v1213[2] + 4))(a1: v1214); /*0x10033bb3*/
              if ( v1215 != 0 ) /*0x10033bb8*/
                break; /*0x10033bb8*/
            }
          }
LABEL_431:
          v1210 = (*(int (__thiscall **)(int))(*(_DWORD *)dword_103EC7B8 + 32))(a1: dword_103EC7B8); /*0x10033b45*/
          v1209 = v1730; /*0x10033b50*/
          if ( (int)v1730 >= v1210 ) /*0x10033b55*/
            goto LABEL_593; /*0x10033b55*/
        }
        switch ( *(_DWORD *)(v1215 + 20) ) /*0x10033bc5*/
        {
          case 1: /*0x10033bc5*/
          case 2: /*0x10033bc5*/
          case 6: /*0x10033bc5*/
          case 0x16: /*0x10033bc5*/
          case 0x17: /*0x10033bc5*/
          case 0x27: /*0x10033bc5*/
          case 0x60: /*0x10033bc5*/
          case 0x74: /*0x10033bc5*/
          case 0x83: /*0x10033bc5*/
          case 0x94: /*0x10033bc5*/
          case 0x98: /*0x10033bc5*/
          case 0x99: /*0x10033bc5*/
          case 0x9A: /*0x10033bc5*/
          case 0x9B: /*0x10033bc5*/
          case 0xA2: /*0x10033bc5*/
          case 0xA3: /*0x10033bc5*/
          case 0xA5: /*0x10033bc5*/
          case 0xA6: /*0x10033bc5*/
          case 0xA9: /*0x10033bc5*/
          case 0xAA: /*0x10033bc5*/
          case 0xAB: /*0x10033bc5*/
          case 0xAC: /*0x10033bc5*/
          case 0xB6: /*0x10033bc5*/
          case 0xE6: /*0x10033bc5*/
            if ( sub_10035F90(a1: v1213) != 0 ) /*0x10033bd5*/
              goto LABEL_431; /*0x10033bd5*/
            if ( (unsigned __int8)sub_1002E000(a1: v1213, a2: v1744, a3: v1741, a4: &v1739, a5: v1742) == 0 ) /*0x10033bfc*/
              goto LABEL_431; /*0x10033bfc*/
            _EAX = (*(int (__thiscall **)(_DWORD *))(*v1213 + 44))(a1: v1213); /*0x10033c06*/
            __asm /*0x10033c09*/
            {
              vmovss xmm0, dword ptr [eax]
              vmovss dword ptr [esi+18h], xmm0
              vmovss xmm0, dword ptr [eax+4]
              vmovss dword ptr [esi+14h], xmm0
              vmovss xmm0, dword ptr [eax+8]
              vmovss dword ptr [esi+40h], xmm0
            }
            (*(void (__thiscall **)(char *))(*(_DWORD *)v1748 + 44))(a1: v1748); /*0x10033c2e*/
            __asm /*0x10033c31*/
            {
              vmovss xmm0, dword ptr [esi+40h]
              vsubss xmm0, xmm0, dword ptr [eax+8]
              vmovss xmm1, dword ptr [esi+14h]
              vsubss xmm1, xmm1, dword ptr [eax+4]
              vmovss xmm2, dword ptr [esi+18h]
              vsubss xmm2, xmm2, dword ptr [eax]
              vmulss xmm1, xmm1, xmm1
              vfmadd231ss xmm1, xmm2, xmm2
              vfmadd231ss xmm1, xmm0, xmm0
              vcvtss2sd xmm0, xmm1, xmm1
              vsqrtsd xmm0, xmm0, xmm0
              vcvtsd2ss xmm0, xmm0, xmm0
              vucomiss xmm0, dword_103F4DF8
            }
            if ( !(v89 | v90) ) /*0x10033c70*/
              goto LABEL_431; /*0x10033c70*/
            i = (char **)sub_1002DFA0(a1: &xmmword_103F4DFC); /*0x10033c83*/
            v1232 = (*(int (__thiscall **)(int))(*(_DWORD *)v1214 + 4))(a1: v1214); /*0x10033c8a*/
            if ( v1232 != 0 ) /*0x10033c8f*/
            {
              v1233 = "Uzi"; /*0x10033ca4*/
              switch ( *(_DWORD *)(v1232 + 20) ) /*0x10033ca9*/
              {
                case 1: /*0x10033ca9*/
                  v1233 = "Assault Rifle"; /*0x10034563*/
                  break; /*0x10034568*/
                case 2: /*0x10033ca9*/
                case 6: /*0x10033ca9*/
                  v1233 = "Auto Shotgun"; /*0x10033cb0*/
                  break; /*0x10033cb5*/
                case 0x27: /*0x10033ca9*/
                  v1233 = "Chainsaw"; /*0x10034527*/
                  break; /*0x1003452c*/
                case 0x60: /*0x10033ca9*/
                  v1233 = "Grenade Launcher"; /*0x10034559*/
                  break; /*0x1003455e*/
                case 0x74: /*0x10033ca9*/
                  v1233 = "Magnum"; /*0x10034513*/
                  break; /*0x10034518*/
                case 0x83: /*0x10033ca9*/
                  v1233 = (char *)"Pistol"; /*0x1003451d*/
                  break; /*0x10034522*/
                case 0x94: /*0x10033ca9*/
                  v1233 = "Pump Shotgun"; /*0x1003453b*/
                  break; /*0x10034540*/
                case 0x98: /*0x10033ca9*/
                  v1233 = "AK-47"; /*0x10034531*/
                  break; /*0x10034536*/
                case 0x99: /*0x10033ca9*/
                  v1233 = "Desert Rifle"; /*0x10034577*/
                  break; /*0x1003457c*/
                case 0x9A: /*0x10033ca9*/
                  v1233 = "M60"; /*0x100344ff*/
                  break; /*0x10034504*/
                case 0x9B: /*0x10033ca9*/
                  v1233 = "SG552"; /*0x100344f5*/
                  break; /*0x100344fa*/
                case 0xA2: /*0x10033ca9*/
                  v1233 = "Chrome Shotgun"; /*0x1003458b*/
                  break; /*0x10034590*/
                case 0xA3: /*0x10033ca9*/
                  v1233 = "SPAS"; /*0x10034545*/
                  break; /*0x1003454a*/
                case 0xA5: /*0x10033ca9*/
                  v1233 = "MP5"; /*0x1003454f*/
                  break; /*0x10034554*/
                case 0xA6: /*0x10033ca9*/
                  v1233 = "Silenced SMG"; /*0x10034581*/
                  break; /*0x10034586*/
                case 0xA9: /*0x10033ca9*/
                  v1233 = "AWP"; /*0x1003456d*/
                  break; /*0x10034572*/
                case 0xAA: /*0x10033ca9*/
                  v1233 = "Military Sniper"; /*0x100344eb*/
                  break; /*0x100344f0*/
                case 0xAB: /*0x10033ca9*/
                  v1233 = "Scout"; /*0x10034509*/
                  break; /*0x1003450e*/
                case 0xAC: /*0x10033ca9*/
                  v1233 = "Hunting Rifle"; /*0x10034595*/
                  break; /*0x1003459a*/
                case 0xB6: /*0x10033ca9*/
                  break;
                default:
                  goto LABEL_458;
              }
            }
            else
            {
LABEL_458:
              v1233 = "Gun"; /*0x10033f76*/
            }
            v1725 = v1233; /*0x10033f7b*/
            sub_1017B960(a1: &v1897); /*0x10033f8b*/
            if ( dword_103F4DE8 != 0 ) /*0x10033f9d*/
            {
              if ( dword_103F4DE8 == 1 && byte_103F4DE6 == 1 ) /*0x10033fb3*/
              {
                v1283 = dword_103F4E4C; /*0x10033fb9*/
                if ( dword_103F4E4C != 0 ) /*0x10033fc0*/
                {
                  if ( dword_103F4E4C == 1 ) /*0x10033fc9*/
                    goto LABEL_464; /*0x10033fc9*/
                }
                else
                {
                  v1734 = *(wchar_t **)ArgList; /*0x10034345*/
                  __asm { vmovss xmm0, dword ptr [esi+90h] } /*0x10034348*/
                  __asm
                  {
                    vcvtsi2ss xmm1, xmm3, eax
                    vsubss xmm0, xmm0, xmm1
                    vaddss xmm0, xmm0, ds:dword_10185850
                    vcvttss2si eax, xmm0
                  }
                  v1727 = _EAX; /*0x10034367*/
                  __asm /*0x1003436a*/
                  {
                    vmovss xmm0, ds:dword_10184EDC
                    vmulss xmm0, xmm0, dword ptr [esi+7Ch]
                    vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                  }
                  v1912 = i; /*0x10034382*/
                  __asm { vcvttss2si edi, xmm0 } /*0x10034388*/
                  sub_1017B960(a1: &v1897); /*0x10034399*/
                  sub_10177250( /*0x100343b8*/
                    a1: (int)v1734,
                    a2: (int)&v1897,
                    a3: _EDI,
                    a4: v1727,
                    a5: (char)v1912,
                    a6: 8,
                    Format: v1725,
                    ArgList: v1724[0]);
                  v1283 = dword_103F4E4C; /*0x100343c0*/
                  if ( dword_103F4E4C == 1 ) /*0x100343c8*/
                  {
LABEL_464:
                    __asm /*0x10033fcf*/
                    {
                      vmovss xmm0, ds:dword_10184EDC
                      vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                      vaddss xmm0, xmm0, dword ptr [esi+90h]
                    }
                    __asm { vcvtsi2ss xmm1, xmm3, eax }
                    v1734 = *(wchar_t **)ArgList; /*0x10033ff9*/
                    __asm /*0x10033ffc*/
                    {
                      vsubss xmm0, xmm0, xmm1
                      vcvttss2si eax, xmm0
                    }
                    v1727 = _EAX; /*0x10034004*/
                    v1911 = i; /*0x1003400a*/
                    __asm /*0x10034010*/
                    {
                      vmovss xmm0, dword ptr [esi+0C0h]
                      vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                      vaddss xmm0, xmm0, ds:dword_1018531C
                      vcvttss2si edi, xmm0
                    }
                    sub_1017B960(a1: &v1897); /*0x10034036*/
                    sub_10177250( /*0x10034055*/
                      a1: (int)v1734,
                      a2: (int)&v1897,
                      a3: _EDI,
                      a4: v1727,
                      a5: (char)v1911,
                      a6: 1,
                      Format: v1725,
                      ArgList: v1724[0]);
                    v1283 = dword_103F4E4C; /*0x1003405d*/
                    if ( dword_103F4E4C != 2 ) /*0x10034065*/
                      goto LABEL_465; /*0x10034065*/
                    goto LABEL_490; /*0x10034065*/
                  }
                }
                if ( v1283 != 2 ) /*0x100343d1*/
                {
LABEL_465:
                  if ( v1283 != 3 ) /*0x1003406e*/
                    goto LABEL_516; /*0x1003406e*/
LABEL_491:
                  v1734 = *(wchar_t **)ArgList; /*0x10034462*/
                  v1909 = i; /*0x1003446d*/
                  __asm /*0x10034473*/
                  {
                    vmovss xmm0, ds:dword_10184EDC
                    vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                    vaddss xmm0, xmm0, dword ptr [esi+90h]
                  }
                  __asm
                  {
                    vcvtsi2ss xmm1, xmm3, eax
                    vsubss xmm0, xmm0, xmm1
                    vcvttss2si ebx, xmm0
                    vmovss xmm0, ds:dword_10185884
                    vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                    vaddss xmm0, xmm0, ds:dword_1018550C
                    vcvttss2si edi, xmm0
                  }
                  sub_1017B960(a1: &v1897); /*0x100344c9*/
                  sub_10177250( /*0x100344e6*/
                    a1: (int)v1734,
                    a2: (int)&v1897,
                    a3: _EDI,
                    a4: _EBX,
                    a5: (char)v1909,
                    a6: 1,
                    Format: v1725,
                    ArgList: v1724[0]);
                  goto LABEL_516; /*0x100344e6*/
                }
LABEL_490:
                __asm /*0x100343d7*/
                {
                  vmovss xmm0, dword ptr [esi+90h]
                  vaddss xmm0, xmm0, dword ptr [esi+0A8h]
                }
                v1734 = *(wchar_t **)ArgList; /*0x100343ec*/
                __asm /*0x100343ef*/
                {
                  vaddss xmm0, xmm0, ds:dword_10185854
                  vcvttss2si eax, xmm0
                }
                v1727 = _EAX; /*0x100343fb*/
                __asm /*0x100343fe*/
                {
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+7Ch]
                  vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                }
                v1910 = i; /*0x10034416*/
                __asm { vcvttss2si edi, xmm0 } /*0x1003441c*/
                sub_1017B960(a1: &v1897); /*0x1003442d*/
                sub_10177250( /*0x1003444c*/
                  a1: (int)v1734,
                  a2: (int)&v1897,
                  a3: _EDI,
                  a4: v1727,
                  a5: (char)v1910,
                  a6: 8,
                  Format: v1725,
                  ArgList: v1724[0]);
                if ( dword_103F4E4C != 3 ) /*0x1003445c*/
                  goto LABEL_516; /*0x1003445c*/
                goto LABEL_491; /*0x1003445c*/
              }
LABEL_516:
              if ( byte_103F4DE5 == 1 ) /*0x10034762*/
              {
                if ( dword_103F4DEC == 1 ) /*0x10034770*/
                {
                  __asm { vcvttss2si eax, dword ptr [esi+0A8h] } /*0x1003488c*/
                  v1725 = _EAX; /*0x10034894*/
                  __asm { vcvttss2si edx, dword ptr [esi+7Ch] } /*0x10034897*/
                  __asm { vcvttss2si edi, dword ptr [esi+90h] }
                  v1429 = (char)i; /*0x100348aa*/
                  v1962 = i; /*0x100348ad*/
                  goto LABEL_579; /*0x100348b3*/
                }
                if ( dword_103F4DEC == 0 ) /*0x10034778*/
                {
                  __asm /*0x1003477e*/
                  {
                    vmovss xmm0, dword ptr [esi+0A8h]
                    vmovss dword ptr [esi+40h], xmm0
                    vcvttss2si eax, xmm0
                  }
                  v1729 = _EAX; /*0x1003478f*/
                  __asm /*0x10034792*/
                  {
                    vmovss xmm0, dword ptr [esi+7Ch]
                    vmovss dword ptr [esi+1Ch], xmm0
                    vcvttss2si edx, xmm0
                  }
                  __asm
                  {
                    vmovss xmm0, dword ptr [esi+90h]
                    vmovss dword ptr [esi+14h], xmm0
                    vcvttss2si edi, xmm0
                  }
                  LOBYTE(_EAX) = (_BYTE)i; /*0x100347b7*/
                  v1963 = i; /*0x100347ba*/
                  __asm /*0x100347c0*/
                  {
                    vmovss xmm0, dword ptr [esi+0C0h]
                    vmovss dword ptr [esi+60h], xmm0
                    vcvttss2si ebx, xmm0
                  }
                  sub_10177840(a1: _EBX, a2: _EDI, a3: _EDX, a4: v1729, a5: _EAX, a6: 1); /*0x100347da*/
                  __asm /*0x100347df*/
                  {
                    vmovss xmm1, ds:dword_10185854
                    vaddss xmm0, xmm1, dword ptr [esi+40h]
                    vcvttss2si eax, xmm0
                  }
                  i = _EAX; /*0x100347f0*/
                  __asm /*0x100347f3*/
                  {
                    vaddss xmm0, xmm1, dword ptr [esi+1Ch]
                    vcvttss2si edx, xmm0
                  }
                  __asm
                  {
                    vmovss xmm1, ds:dword_1018555C
                    vaddss xmm0, xmm1, dword ptr [esi+14h]
                    vcvttss2si edi, xmm0
                  }
                  v1904 = -16777216; /*0x10034813*/
                  __asm /*0x1003481d*/
                  {
                    vaddss xmm0, xmm1, dword ptr [esi+60h]
                    vcvttss2si ebx, xmm0
                  }
                  sub_10177840(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: 0, a6: 1); /*0x10034835*/
                  __asm /*0x1003483a*/
                  {
                    vmovss xmm1, ds:dword_10185850
                    vaddss xmm0, xmm1, dword ptr [esi+40h]
                    vcvttss2si eax, xmm0
                  }
                  i = _EAX; /*0x1003484b*/
                  __asm /*0x1003484e*/
                  {
                    vaddss xmm0, xmm1, dword ptr [esi+1Ch]
                    vcvttss2si edx, xmm0
                  }
                  __asm
                  {
                    vmovss xmm1, ds:dword_10184184
                    vaddss xmm0, xmm1, dword ptr [esi+14h]
                    vcvttss2si edi, xmm0
                  }
                  v1903 = -16777216; /*0x1003486e*/
                  __asm /*0x10034878*/
                  {
                    vaddss xmm0, xmm1, dword ptr [esi+60h]
                    vcvttss2si ebx, xmm0
                  }
                  goto LABEL_577; /*0x10034887*/
                }
              }
              goto LABEL_431; /*0x10034778*/
            }
            v1734 = (wchar_t *)v1898; /*0x10034079*/
            v1294 = (*(int (__thiscall **)(int))(*(_DWORD *)v1214 + 4))(a1: v1214); /*0x10034086*/
            if ( v1294 != 0 ) /*0x1003408b*/
              v1295 = *(_DWORD *)(v1294 + 20); /*0x1003408d*/
            else
              v1295 = 0; /*0x10034092*/
            v1296 = sub_10166390(a1: v1295); /*0x10034097*/
            v1297 = v1296 & ~(v1296 >> 31); /*0x100340a1*/
            if ( v1297 >= 38 ) /*0x100340ae*/
              v1297 = 38; /*0x100340ae*/
            v1298 = sub_10168E50(a1: v1297); /*0x100340b8*/
            if ( v1298 != 0 ) /*0x100340bf*/
            {
              v1299 = v1298; /*0x100340c5*/
              v1300 = dword_103F4E4C; /*0x100340c7*/
              if ( dword_103F4E4C != 0 ) /*0x100340ce*/
              {
                if ( dword_103F4E4C == 2 ) /*0x100340d7*/
                  goto LABEL_475; /*0x100340d7*/
              }
              else
              {
                v1967 = i; /*0x10034223*/
                __asm /*0x10034229*/
                {
                  vmovss xmm0, ds:dword_10185888
                  vaddss xmm0, xmm0, dword ptr [esi+90h]
                  vaddss xmm0, xmm0, ds:dword_10185850
                  vcvttss2si eax, xmm0
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+7Ch]
                  vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                  vcvttss2si edx, xmm0
                }
                sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v1299, a6: 8, a7: (char)i); /*0x10034268*/
                v1300 = dword_103F4E4C; /*0x1003426d*/
                if ( dword_103F4E4C == 2 ) /*0x10034275*/
                {
LABEL_475:
                  v1966 = i; /*0x100340dd*/
                  __asm /*0x100340ec*/
                  {
                    vmovss xmm0, dword ptr [esi+90h]
                    vaddss xmm0, xmm0, dword ptr [esi+0A8h]
                    vaddss xmm0, xmm0, ds:dword_10185854
                    vcvttss2si eax, xmm0
                    vmovss xmm0, ds:dword_10184EDC
                    vmulss xmm0, xmm0, dword ptr [esi+7Ch]
                    vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                    vcvttss2si edx, xmm0
                  }
                  sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v1299, a6: 8, a7: (char)i); /*0x1003412b*/
                  v1300 = dword_103F4E4C; /*0x10034130*/
                  if ( dword_103F4E4C != 1 ) /*0x10034138*/
                    goto LABEL_476; /*0x10034138*/
                  goto LABEL_486; /*0x10034138*/
                }
              }
              if ( v1300 != 1 ) /*0x1003427e*/
              {
LABEL_476:
                if ( v1300 != 3 ) /*0x10034141*/
                  goto LABEL_516; /*0x10034141*/
LABEL_487:
                __asm /*0x100342e5*/
                {
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                  vaddss xmm0, xmm0, dword ptr [esi+90h]
                  vcvttss2si eax, xmm0
                  vmovss xmm0, ds:dword_10185888
                  vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                }
                v1964 = i; /*0x1003431a*/
                __asm /*0x10034320*/
                {
                  vaddss xmm0, xmm0, ds:dword_10185850
                  vcvttss2si edx, xmm0
                }
                sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v1299, a6: 16, a7: (char)i); /*0x10034336*/
                goto LABEL_516; /*0x1003433b*/
              }
LABEL_486:
              __asm /*0x10034284*/
              {
                vmovss xmm0, ds:dword_10184EDC
                vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                vaddss xmm0, xmm0, dword ptr [esi+90h]
                vcvttss2si eax, xmm0
              }
              __asm
              {
                vmovss xmm0, dword ptr [esi+0C0h]
                vaddss xmm0, xmm0, dword ptr [esi+7Ch]
              }
              v1965 = i; /*0x100342b6*/
              __asm /*0x100342bc*/
              {
                vaddss xmm0, xmm0, ds:dword_10185854
                vcvttss2si edx, xmm0
              }
              sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v1299, a6: 16, a7: (char)i); /*0x100342d2*/
              if ( dword_103F4E4C != 3 ) /*0x100342df*/
                goto LABEL_516; /*0x100342df*/
              goto LABEL_487; /*0x100342df*/
            }
            if ( byte_103F4DE6 != 1 ) /*0x10034153*/
              goto LABEL_516; /*0x10034153*/
            v1309 = dword_103F4E4C; /*0x10034159*/
            if ( dword_103F4E4C != 0 ) /*0x10034160*/
            {
              if ( dword_103F4E4C == 1 ) /*0x10034169*/
                goto LABEL_481; /*0x10034169*/
            }
            else
            {
              v1727 = *(_DWORD *)ArgList; /*0x100345b8*/
              __asm { vmovss xmm0, dword ptr [esi+90h] } /*0x100345bb*/
              __asm
              {
                vcvtsi2ss xmm1, xmm3, eax
                vsubss xmm0, xmm0, xmm1
                vaddss xmm0, xmm0, ds:dword_10185850
                vcvttss2si ebx, xmm0
                vmovss xmm0, ds:dword_10184EDC
                vmulss xmm0, xmm0, dword ptr [esi+7Ch]
                vaddss xmm0, xmm0, dword ptr [esi+0C0h]
              }
              v1908 = i; /*0x100345f3*/
              __asm { vcvttss2si edi, xmm0 } /*0x100345f9*/
              sub_1017B960(a1: &v1897); /*0x1003460a*/
              sub_10177250( /*0x10034627*/
                a1: v1727,
                a2: (int)&v1897,
                a3: _EDI,
                a4: _EBX,
                a5: (char)v1908,
                a6: 8,
                Format: v1725,
                ArgList: v1724[0]);
              v1309 = dword_103F4E4C; /*0x1003462f*/
              if ( dword_103F4E4C == 1 ) /*0x10034637*/
              {
LABEL_481:
                __asm /*0x1003416f*/
                {
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                  vaddss xmm0, xmm0, dword ptr [esi+90h]
                }
                __asm { vcvtsi2ss xmm1, xmm3, eax }
                v1727 = *(_DWORD *)ArgList; /*0x1003419c*/
                __asm /*0x1003419f*/
                {
                  vsubss xmm0, xmm0, xmm1
                  vcvttss2si ebx, xmm0
                }
                v1907 = i; /*0x100341aa*/
                __asm /*0x100341b0*/
                {
                  vmovss xmm0, dword ptr [esi+0C0h]
                  vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                  vaddss xmm0, xmm0, ds:dword_1018531C
                  vcvttss2si edi, xmm0
                }
                sub_1017B960(a1: &v1897); /*0x100341d6*/
                sub_10177250( /*0x100341f3*/
                  a1: v1727,
                  a2: (int)&v1897,
                  a3: _EDI,
                  a4: _EBX,
                  a5: (char)v1907,
                  a6: 1,
                  Format: v1725,
                  ArgList: v1724[0]);
                v1309 = dword_103F4E4C; /*0x100341fb*/
                if ( dword_103F4E4C != 2 ) /*0x10034203*/
                  goto LABEL_482; /*0x10034203*/
                goto LABEL_514; /*0x10034203*/
              }
            }
            if ( v1309 != 2 ) /*0x10034640*/
            {
LABEL_482:
              if ( v1309 != 3 ) /*0x1003420f*/
                goto LABEL_516; /*0x1003420f*/
LABEL_515:
              v1727 = *(_DWORD *)ArgList; /*0x100346cf*/
              v1905 = i; /*0x100346da*/
              __asm /*0x100346e0*/
              {
                vmovss xmm0, ds:dword_10184EDC
                vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                vaddss xmm0, xmm0, dword ptr [esi+90h]
              }
              __asm
              {
                vcvtsi2ss xmm1, xmm3, eax
                vsubss xmm0, xmm0, xmm1
                vcvttss2si ebx, xmm0
                vmovss xmm0, ds:dword_10185884
                vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                vaddss xmm0, xmm0, ds:dword_1018550C
                vcvttss2si edi, xmm0
              }
              sub_1017B960(a1: &v1897); /*0x10034736*/
              sub_10177250( /*0x10034753*/
                a1: v1727,
                a2: (int)&v1897,
                a3: _EDI,
                a4: _EBX,
                a5: (char)v1905,
                a6: 1,
                Format: v1725,
                ArgList: v1724[0]);
              goto LABEL_516; /*0x10034753*/
            }
LABEL_514:
            __asm /*0x10034646*/
            {
              vmovss xmm0, dword ptr [esi+90h]
              vaddss xmm0, xmm0, dword ptr [esi+0A8h]
            }
            v1727 = *(_DWORD *)ArgList; /*0x1003465b*/
            __asm /*0x1003465e*/
            {
              vaddss xmm0, xmm0, ds:dword_10185854
              vcvttss2si ebx, xmm0
              vmovss xmm0, ds:dword_10184EDC
              vmulss xmm0, xmm0, dword ptr [esi+7Ch]
              vaddss xmm0, xmm0, dword ptr [esi+0C0h]
            }
            v1906 = i; /*0x10034682*/
            __asm { vcvttss2si edi, xmm0 } /*0x10034688*/
            sub_1017B960(a1: &v1897); /*0x10034699*/
            sub_10177250( /*0x100346b6*/
              a1: v1727,
              a2: (int)&v1897,
              a3: _EDI,
              a4: _EBX,
              a5: (char)v1906,
              a6: 8,
              Format: v1725,
              ArgList: v1724[0]);
            if ( dword_103F4E4C != 3 ) /*0x100346c9*/
              goto LABEL_516; /*0x100346c9*/
            goto LABEL_515; /*0x100346c9*/
          case 0x49: /*0x10033bc5*/
          case 0x69: /*0x10033bc5*/
          case 0x6A: /*0x10033bc5*/
          case 0x6D: /*0x10033bc5*/
          case 0x6E: /*0x10033bc5*/
          case 0x6F: /*0x10033bc5*/
          case 0x76: /*0x10033bc5*/
          case 0x79: /*0x10033bc5*/
          case 0x81: /*0x10033bc5*/
            if ( sub_10035F90(a1: v1213) != 0 ) /*0x10033b3f*/
              goto LABEL_431; /*0x10033b3f*/
            if ( (unsigned __int8)sub_1002E000(a1: v1213, a2: v1744, a3: v1741, a4: &v1739, a5: v1742) == 0 ) /*0x10033cdb*/
              goto LABEL_431; /*0x10033cdb*/
            _EAX = (*(int (__thiscall **)(_DWORD *))(*v1213 + 44))(a1: v1213); /*0x10033ce5*/
            __asm /*0x10033ce8*/
            {
              vmovss xmm0, dword ptr [eax]
              vmovss dword ptr [esi+18h], xmm0
              vmovss xmm0, dword ptr [eax+4]
              vmovss dword ptr [esi+14h], xmm0
              vmovss xmm0, dword ptr [eax+8]
              vmovss dword ptr [esi+40h], xmm0
            }
            (*(void (__thiscall **)(char *))(*(_DWORD *)v1748 + 44))(a1: v1748); /*0x10033d0d*/
            __asm /*0x10033d10*/
            {
              vmovss xmm0, dword ptr [esi+40h]
              vsubss xmm0, xmm0, dword ptr [eax+8]
              vmovss xmm1, dword ptr [esi+14h]
              vsubss xmm1, xmm1, dword ptr [eax+4]
              vmovss xmm2, dword ptr [esi+18h]
              vsubss xmm2, xmm2, dword ptr [eax]
              vmulss xmm1, xmm1, xmm1
              vfmadd231ss xmm1, xmm2, xmm2
              vfmadd231ss xmm1, xmm0, xmm0
              vcvtss2sd xmm0, xmm1, xmm1
              vsqrtsd xmm0, xmm0, xmm0
              vcvtsd2ss xmm0, xmm0, xmm0
              vucomiss xmm0, dword_103F4DF8
            }
            if ( !(v89 | v90) ) /*0x10033d4f*/
              goto LABEL_431; /*0x10033d4f*/
            i = (char **)sub_1002DFA0(a1: &xmmword_103F4E0C); /*0x10033d62*/
            v1250 = (*(int (__thiscall **)(int))(*(_DWORD *)v1214 + 4))(a1: v1214); /*0x10033d69*/
            v1251 = (char *)&Buf; /*0x10033d6c*/
            if ( v1250 != 0 ) /*0x10033d73*/
            {
              v1252 = *(_DWORD *)(v1250 + 20) - 73; /*0x10033d7c*/
              v1253 = "Adrenaline"; /*0x10033d88*/
              v1254 = &xmmword_103F4E0C; /*0x10033d8d*/
              switch ( v1252 ) /*0x10033d92*/
              {
                case 0: /*0x10033d92*/
                  v1253 = "First Aid Kit"; /*0x10033d99*/
                  goto LABEL_528; /*0x10033d9e*/
                case 32: /*0x10033d92*/
                  goto LABEL_528;
                case 33: /*0x10033d92*/
                  v1253 = "Bile Bomb"; /*0x1003459f*/
                  goto LABEL_527; /*0x100345a4*/
                case 36: /*0x10033d92*/
                  v1253 = "Defibrillator"; /*0x100348c6*/
                  goto LABEL_528; /*0x100348cb*/
                case 37: /*0x10033d92*/
                  v1253 = "Explosive Ammo"; /*0x100348cd*/
                  goto LABEL_525; /*0x100348cd*/
                case 38: /*0x10033d92*/
                  v1253 = "Incendiary Ammo"; /*0x100345a9*/
LABEL_525:
                  v1254 = &xmmword_103F4E2C; /*0x100348d2*/
                  goto LABEL_528; /*0x100348d7*/
                case 45: /*0x10033d92*/
                  v1253 = "Molotov"; /*0x100348b8*/
                  goto LABEL_527; /*0x100348bd*/
                case 48: /*0x10033d92*/
                  v1253 = "Pain Pills"; /*0x100348bf*/
                  goto LABEL_528; /*0x100348c4*/
                case 56: /*0x10033d92*/
                  v1253 = "Pipe Bomb"; /*0x100348d9*/
LABEL_527:
                  v1254 = &xmmword_103F4E1C; /*0x100348de*/
LABEL_528:
                  i = (char **)sub_1002DFA0(a1: v1254); /*0x100348e3*/
                  v1251 = (char *)v1253; /*0x100348ef*/
                  break; /*0x100348ef*/
                default:
                  break;
              }
            }
            v1725 = v1251; /*0x100348f1*/
            sub_1017B960(a1: &v1897); /*0x10034901*/
            v1734 = (wchar_t *)v1898; /*0x1003490c*/
            if ( dword_103F4DE8 != 0 ) /*0x10034916*/
            {
              if ( dword_103F4DE8 != 1 || byte_103F4DE6 != 1 ) /*0x1003492c*/
                goto LABEL_573; /*0x1003492c*/
              v1430 = dword_103F4E4C; /*0x10034932*/
              if ( dword_103F4E4C != 0 ) /*0x10034939*/
              {
                if ( dword_103F4E4C == 1 ) /*0x10034942*/
                  goto LABEL_534; /*0x10034942*/
LABEL_559:
                if ( v1430 == 2 ) /*0x10034d3e*/
                  goto LABEL_560; /*0x10034d3e*/
LABEL_535:
                if ( v1430 != 3 ) /*0x100349e5*/
                  goto LABEL_573; /*0x100349e5*/
              }
              else
              {
                v1727 = *(_DWORD *)ArgList; /*0x10034cb6*/
                __asm { vmovss xmm0, dword ptr [esi+90h] } /*0x10034cb9*/
                __asm
                {
                  vcvtsi2ss xmm1, xmm3, eax
                  vsubss xmm0, xmm0, xmm1
                  vaddss xmm0, xmm0, ds:dword_10185850
                  vcvttss2si ebx, xmm0
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+7Ch]
                  vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                }
                v1928 = i; /*0x10034cf1*/
                __asm { vcvttss2si edi, xmm0 } /*0x10034cf7*/
                sub_1017B960(a1: &v1897); /*0x10034d08*/
                sub_10177250( /*0x10034d25*/
                  a1: v1727,
                  a2: (int)&v1897,
                  a3: _EDI,
                  a4: _EBX,
                  a5: (char)v1928,
                  a6: 8,
                  Format: v1725,
                  ArgList: v1724[0]);
                v1430 = dword_103F4E4C; /*0x10034d2d*/
                if ( dword_103F4E4C != 1 ) /*0x10034d35*/
                  goto LABEL_559; /*0x10034d35*/
LABEL_534:
                __asm /*0x10034948*/
                {
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                  vaddss xmm0, xmm0, dword ptr [esi+90h]
                }
                __asm { vcvtsi2ss xmm1, xmm3, eax }
                v1727 = *(_DWORD *)ArgList; /*0x10034975*/
                __asm /*0x10034978*/
                {
                  vsubss xmm0, xmm0, xmm1
                  vcvttss2si ebx, xmm0
                }
                v1927 = i; /*0x10034983*/
                __asm /*0x10034989*/
                {
                  vmovss xmm0, dword ptr [esi+0C0h]
                  vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                  vaddss xmm0, xmm0, ds:dword_1018531C
                  vcvttss2si edi, xmm0
                }
                sub_1017B960(a1: &v1897); /*0x100349af*/
                sub_10177250( /*0x100349cc*/
                  a1: v1727,
                  a2: (int)&v1897,
                  a3: _EDI,
                  a4: _EBX,
                  a5: (char)v1927,
                  a6: 1,
                  Format: v1725,
                  ArgList: v1724[0]);
                v1430 = dword_103F4E4C; /*0x100349d4*/
                if ( dword_103F4E4C != 2 ) /*0x100349dc*/
                  goto LABEL_535; /*0x100349dc*/
LABEL_560:
                __asm /*0x10034d44*/
                {
                  vmovss xmm0, dword ptr [esi+90h]
                  vaddss xmm0, xmm0, dword ptr [esi+0A8h]
                }
                v1727 = *(_DWORD *)ArgList; /*0x10034d59*/
                __asm /*0x10034d5c*/
                {
                  vaddss xmm0, xmm0, ds:dword_10185854
                  vcvttss2si ebx, xmm0
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+7Ch]
                  vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                }
                v1926 = i; /*0x10034d80*/
                __asm { vcvttss2si edi, xmm0 } /*0x10034d86*/
                sub_1017B960(a1: &v1897); /*0x10034d97*/
                sub_10177250( /*0x10034db4*/
                  a1: v1727,
                  a2: (int)&v1897,
                  a3: _EDI,
                  a4: _EBX,
                  a5: (char)v1926,
                  a6: 8,
                  Format: v1725,
                  ArgList: v1724[0]);
                if ( dword_103F4E4C != 3 ) /*0x10034dc4*/
                {
LABEL_573:
                  if ( byte_103F4DE5 == 1 ) /*0x1003508e*/
                  {
                    if ( dword_103F4DEC == 1 ) /*0x1003509c*/
                    {
                      __asm { vcvttss2si eax, dword ptr [esi+0A8h] } /*0x100351c6*/
                      v1725 = _EAX; /*0x100351ce*/
                      __asm { vcvttss2si edx, dword ptr [esi+7Ch] } /*0x100351d1*/
                      __asm { vcvttss2si edi, dword ptr [esi+90h] }
                      v1429 = (char)i; /*0x100351e4*/
                      v1974 = i; /*0x100351e7*/
                      goto LABEL_579; /*0x100351e7*/
                    }
                    if ( dword_103F4DEC == 0 ) /*0x100350a4*/
                    {
                      __asm /*0x100350aa*/
                      {
                        vmovss xmm0, dword ptr [esi+0A8h]
                        vmovss dword ptr [esi+40h], xmm0
                        vcvttss2si eax, xmm0
                      }
                      v1729 = _EAX; /*0x100350bb*/
                      __asm /*0x100350be*/
                      {
                        vmovss xmm0, dword ptr [esi+7Ch]
                        vmovss dword ptr [esi+1Ch], xmm0
                        vcvttss2si edx, xmm0
                      }
                      __asm
                      {
                        vmovss xmm0, dword ptr [esi+90h]
                        vmovss dword ptr [esi+14h], xmm0
                        vcvttss2si edi, xmm0
                      }
                      LOBYTE(_EAX) = (_BYTE)i; /*0x100350e3*/
                      v1975 = i; /*0x100350e6*/
                      __asm /*0x100350ec*/
                      {
                        vmovss xmm0, dword ptr [esi+0C0h]
                        vmovss dword ptr [esi+60h], xmm0
                        vcvttss2si ebx, xmm0
                      }
                      sub_10177840(a1: _EBX, a2: _EDI, a3: _EDX, a4: v1729, a5: _EAX, a6: 1); /*0x10035106*/
                      __asm /*0x1003510b*/
                      {
                        vmovss xmm1, ds:dword_10185854
                        vaddss xmm0, xmm1, dword ptr [esi+40h]
                        vcvttss2si eax, xmm0
                      }
                      i = _EAX; /*0x1003511c*/
                      __asm /*0x1003511f*/
                      {
                        vaddss xmm0, xmm1, dword ptr [esi+1Ch]
                        vcvttss2si edx, xmm0
                      }
                      __asm
                      {
                        vmovss xmm1, ds:dword_1018555C
                        vaddss xmm0, xmm1, dword ptr [esi+14h]
                        vcvttss2si edi, xmm0
                      }
                      v1920 = -16777216; /*0x1003513f*/
                      __asm /*0x10035149*/
                      {
                        vaddss xmm0, xmm1, dword ptr [esi+60h]
                        vcvttss2si ebx, xmm0
                      }
                      sub_10177840(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: 0, a6: 1); /*0x10035161*/
                      __asm /*0x10035166*/
                      {
                        vmovss xmm1, ds:dword_10185850
                        vaddss xmm0, xmm1, dword ptr [esi+40h]
                        vcvttss2si eax, xmm0
                      }
                      i = _EAX; /*0x10035177*/
                      __asm /*0x1003517a*/
                      {
                        vaddss xmm0, xmm1, dword ptr [esi+1Ch]
                        vcvttss2si edx, xmm0
                      }
                      __asm
                      {
                        vmovss xmm1, ds:dword_10184184
                        vaddss xmm0, xmm1, dword ptr [esi+14h]
                        vcvttss2si edi, xmm0
                      }
                      v1919 = -16777216; /*0x1003519a*/
                      __asm /*0x100351a4*/
                      {
                        vaddss xmm0, xmm1, dword ptr [esi+60h]
                        vcvttss2si ebx, xmm0
                      }
                      goto LABEL_577; /*0x100351a9*/
                    }
                  }
                  goto LABEL_431; /*0x100350a4*/
                }
              }
              v1727 = *(_DWORD *)ArgList; /*0x10034dcf*/
              v1925 = i; /*0x10034dd5*/
              __asm /*0x10034ddb*/
              {
                vmovss xmm0, ds:dword_10184EDC
                vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                vaddss xmm0, xmm0, dword ptr [esi+90h]
              }
              __asm
              {
                vcvtsi2ss xmm1, xmm3, eax
                vsubss xmm0, xmm0, xmm1
                vcvttss2si ebx, xmm0
                vmovss xmm0, ds:dword_10185884
                vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                vaddss xmm0, xmm0, ds:dword_1018550C
                vcvttss2si edi, xmm0
              }
              sub_1017B960(a1: &v1897); /*0x10034e34*/
              v1518 = (char)v1925; /*0x10034e39*/
LABEL_572:
              sub_10177250( /*0x1003506d*/
                a1: v1727,
                a2: (int)&v1897,
                a3: _EDI,
                a4: _EBX,
                a5: v1518,
                a6: 1,
                Format: v1725,
                ArgList: v1724[0]);
              goto LABEL_573; /*0x1003507f*/
            }
            v1441 = (*(int (__thiscall **)(int))(*(_DWORD *)v1214 + 4))(a1: v1214); /*0x100349fa*/
            if ( v1441 != 0 ) /*0x100349ff*/
              v1442 = *(_DWORD *)(v1441 + 20); /*0x10034a01*/
            else
              v1442 = 0; /*0x10034a06*/
            v1443 = sub_10166390(a1: v1442); /*0x10034a0b*/
            v1444 = v1443 & ~(v1443 >> 31); /*0x10034a15*/
            if ( v1444 >= 38 ) /*0x10034a22*/
              v1444 = 38; /*0x10034a22*/
            v1445 = sub_10168E50(a1: v1444); /*0x10034a2c*/
            if ( v1445 != 0 ) /*0x10034a33*/
            {
              v1446 = v1445; /*0x10034a39*/
              v1447 = dword_103F4E4C; /*0x10034a3b*/
              if ( dword_103F4E4C != 0 ) /*0x10034a42*/
              {
                if ( dword_103F4E4C == 2 ) /*0x10034a4b*/
                  goto LABEL_545; /*0x10034a4b*/
LABEL_555:
                if ( v1447 == 1 ) /*0x10034bef*/
                  goto LABEL_556; /*0x10034bef*/
LABEL_546:
                if ( v1447 != 3 ) /*0x10034ab5*/
                  goto LABEL_573; /*0x10034ab5*/
              }
              else
              {
                v1979 = i; /*0x10034b94*/
                __asm /*0x10034b9a*/
                {
                  vmovss xmm0, ds:dword_10185888
                  vaddss xmm0, xmm0, dword ptr [esi+90h]
                  vaddss xmm0, xmm0, ds:dword_10185850
                  vcvttss2si eax, xmm0
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+7Ch]
                  vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                  vcvttss2si edx, xmm0
                }
                sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v1446, a6: 8, a7: (char)i); /*0x10034bd9*/
                v1447 = dword_103F4E4C; /*0x10034bde*/
                if ( dword_103F4E4C != 2 ) /*0x10034be6*/
                  goto LABEL_555; /*0x10034be6*/
LABEL_545:
                v1978 = i; /*0x10034a51*/
                __asm /*0x10034a60*/
                {
                  vmovss xmm0, dword ptr [esi+90h]
                  vaddss xmm0, xmm0, dword ptr [esi+0A8h]
                  vaddss xmm0, xmm0, ds:dword_10185854
                  vcvttss2si eax, xmm0
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+7Ch]
                  vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                  vcvttss2si edx, xmm0
                }
                sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v1446, a6: 8, a7: (char)i); /*0x10034a9f*/
                v1447 = dword_103F4E4C; /*0x10034aa4*/
                if ( dword_103F4E4C != 1 ) /*0x10034aac*/
                  goto LABEL_546; /*0x10034aac*/
LABEL_556:
                __asm /*0x10034bf5*/
                {
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                  vaddss xmm0, xmm0, dword ptr [esi+90h]
                  vcvttss2si eax, xmm0
                }
                __asm
                {
                  vmovss xmm0, dword ptr [esi+0C0h]
                  vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                }
                v1977 = i; /*0x10034c27*/
                __asm /*0x10034c2d*/
                {
                  vaddss xmm0, xmm0, ds:dword_10185854
                  vcvttss2si edx, xmm0
                }
                sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v1446, a6: 16, a7: (char)i); /*0x10034c43*/
                if ( dword_103F4E4C != 3 ) /*0x10034c50*/
                  goto LABEL_573; /*0x10034c50*/
              }
              __asm /*0x10034c5c*/
              {
                vmovss xmm0, ds:dword_10184EDC
                vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                vaddss xmm0, xmm0, dword ptr [esi+90h]
                vcvttss2si eax, xmm0
                vmovss xmm0, ds:dword_10185888
                vaddss xmm0, xmm0, dword ptr [esi+0C0h]
              }
              v1976 = i; /*0x10034c8b*/
              __asm /*0x10034c91*/
              {
                vaddss xmm0, xmm0, ds:dword_10185850
                vcvttss2si edx, xmm0
              }
              sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v1446, a6: 16, a7: (char)i); /*0x10034ca7*/
              goto LABEL_573; /*0x10034cac*/
            }
            if ( byte_103F4DE6 != 1 ) /*0x10034ac7*/
              goto LABEL_573; /*0x10034ac7*/
            v1456 = dword_103F4E4C; /*0x10034acd*/
            if ( dword_103F4E4C != 0 ) /*0x10034ad4*/
            {
              if ( dword_103F4E4C == 1 ) /*0x10034add*/
                goto LABEL_551; /*0x10034add*/
LABEL_569:
              if ( v1456 == 2 ) /*0x10034f6c*/
                goto LABEL_570; /*0x10034f6c*/
LABEL_552:
              if ( v1456 != 3 ) /*0x10034b80*/
                goto LABEL_573; /*0x10034b80*/
            }
            else
            {
              v1727 = *(_DWORD *)ArgList; /*0x10034ee4*/
              __asm { vmovss xmm0, dword ptr [esi+90h] } /*0x10034ee7*/
              __asm
              {
                vcvtsi2ss xmm1, xmm3, eax
                vsubss xmm0, xmm0, xmm1
                vaddss xmm0, xmm0, ds:dword_10185850
                vcvttss2si ebx, xmm0
                vmovss xmm0, ds:dword_10184EDC
                vmulss xmm0, xmm0, dword ptr [esi+7Ch]
                vaddss xmm0, xmm0, dword ptr [esi+0C0h]
              }
              v1924 = i; /*0x10034f1f*/
              __asm { vcvttss2si edi, xmm0 } /*0x10034f25*/
              sub_1017B960(a1: &v1897); /*0x10034f36*/
              sub_10177250( /*0x10034f53*/
                a1: v1727,
                a2: (int)&v1897,
                a3: _EDI,
                a4: _EBX,
                a5: (char)v1924,
                a6: 8,
                Format: v1725,
                ArgList: v1724[0]);
              v1456 = dword_103F4E4C; /*0x10034f5b*/
              if ( dword_103F4E4C != 1 ) /*0x10034f63*/
                goto LABEL_569; /*0x10034f63*/
LABEL_551:
              __asm /*0x10034ae3*/
              {
                vmovss xmm0, ds:dword_10184EDC
                vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                vaddss xmm0, xmm0, dword ptr [esi+90h]
              }
              __asm { vcvtsi2ss xmm1, xmm3, eax }
              v1727 = *(_DWORD *)ArgList; /*0x10034b10*/
              __asm /*0x10034b13*/
              {
                vsubss xmm0, xmm0, xmm1
                vcvttss2si ebx, xmm0
              }
              v1923 = i; /*0x10034b1e*/
              __asm /*0x10034b24*/
              {
                vmovss xmm0, dword ptr [esi+0C0h]
                vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                vaddss xmm0, xmm0, ds:dword_1018531C
                vcvttss2si edi, xmm0
              }
              sub_1017B960(a1: &v1897); /*0x10034b4a*/
              sub_10177250( /*0x10034b67*/
                a1: v1727,
                a2: (int)&v1897,
                a3: _EDI,
                a4: _EBX,
                a5: (char)v1923,
                a6: 1,
                Format: v1725,
                ArgList: v1724[0]);
              v1456 = dword_103F4E4C; /*0x10034b6f*/
              if ( dword_103F4E4C != 2 ) /*0x10034b77*/
                goto LABEL_552; /*0x10034b77*/
LABEL_570:
              __asm /*0x10034f72*/
              {
                vmovss xmm0, dword ptr [esi+90h]
                vaddss xmm0, xmm0, dword ptr [esi+0A8h]
              }
              v1727 = *(_DWORD *)ArgList; /*0x10034f87*/
              __asm /*0x10034f8a*/
              {
                vaddss xmm0, xmm0, ds:dword_10185854
                vcvttss2si ebx, xmm0
                vmovss xmm0, ds:dword_10184EDC
                vmulss xmm0, xmm0, dword ptr [esi+7Ch]
                vaddss xmm0, xmm0, dword ptr [esi+0C0h]
              }
              v1922 = i; /*0x10034fae*/
              __asm { vcvttss2si edi, xmm0 } /*0x10034fb4*/
              sub_1017B960(a1: &v1897); /*0x10034fc5*/
              sub_10177250( /*0x10034fe2*/
                a1: v1727,
                a2: (int)&v1897,
                a3: _EDI,
                a4: _EBX,
                a5: (char)v1922,
                a6: 8,
                Format: v1725,
                ArgList: v1724[0]);
              if ( dword_103F4E4C != 3 ) /*0x10034ff2*/
                goto LABEL_573; /*0x10034ff2*/
            }
            v1727 = *(_DWORD *)ArgList; /*0x10034ffd*/
            v1921 = i; /*0x10035003*/
            __asm /*0x10035009*/
            {
              vmovss xmm0, ds:dword_10184EDC
              vmulss xmm0, xmm0, dword ptr [esi+0A8h]
              vaddss xmm0, xmm0, dword ptr [esi+90h]
            }
            __asm
            {
              vcvtsi2ss xmm1, xmm3, eax
              vsubss xmm0, xmm0, xmm1
              vcvttss2si ebx, xmm0
              vmovss xmm0, ds:dword_10185884
              vaddss xmm0, xmm0, dword ptr [esi+0C0h]
              vaddss xmm0, xmm0, ds:dword_1018550C
              vcvttss2si edi, xmm0
            }
            sub_1017B960(a1: &v1897); /*0x10035062*/
            v1518 = (char)v1921; /*0x10035067*/
            goto LABEL_572; /*0x10035067*/
          case 0xE7: /*0x10033bc5*/
            if ( *(_DWORD *)sub_10036090(a1: v1213) != -1 ) /*0x10033dad*/
              goto LABEL_431; /*0x10033dad*/
            if ( (unsigned __int8)sub_1002E000(a1: v1213, a2: v1744, a3: v1741, a4: &v1739, a5: v1742) == 0 ) /*0x10033dd4*/
              goto LABEL_431; /*0x10033dd4*/
            _EAX = (*(int (__thiscall **)(_DWORD *))(*v1213 + 44))(a1: v1213); /*0x10033dde*/
            __asm /*0x10033de1*/
            {
              vmovss xmm0, dword ptr [eax]
              vmovss dword ptr [esi+18h], xmm0
              vmovss xmm0, dword ptr [eax+4]
              vmovss dword ptr [esi+14h], xmm0
              vmovss xmm0, dword ptr [eax+8]
              vmovss dword ptr [esi+40h], xmm0
            }
            (*(void (__thiscall **)(char *))(*(_DWORD *)v1748 + 44))(a1: v1748); /*0x10033e06*/
            __asm /*0x10033e09*/
            {
              vmovss xmm0, dword ptr [esi+40h]
              vsubss xmm0, xmm0, dword ptr [eax+8]
              vmovss xmm1, dword ptr [esi+14h]
              vsubss xmm1, xmm1, dword ptr [eax+4]
              vmovss xmm2, dword ptr [esi+18h]
              vsubss xmm2, xmm2, dword ptr [eax]
              vmulss xmm1, xmm1, xmm1
              vfmadd231ss xmm1, xmm2, xmm2
              vfmadd231ss xmm1, xmm0, xmm0
              vcvtss2sd xmm0, xmm1, xmm1
              vsqrtsd xmm0, xmm0, xmm0
              vcvtsd2ss xmm0, xmm0, xmm0
              vucomiss xmm0, dword_103F4DF8
            }
            if ( !(v89 | v90) ) /*0x10033e48*/
              goto LABEL_431; /*0x10033e48*/
            i = (char **)sub_1002DFA0(a1: &xmmword_103F4E3C); /*0x10033e5b*/
            v1271 = sub_10168120(a1: v1213); /*0x10033e6a*/
            v1725 = (char *)sub_10168500(a1: v1271); /*0x10033e78*/
            sub_1017B960(a1: &v1897); /*0x10033e88*/
            if ( dword_103F4DE8 != 0 ) /*0x10033e94*/
            {
              if ( dword_103F4DE8 != 1 || byte_103F4DE6 != 1 ) /*0x10033eaa*/
                goto LABEL_588; /*0x10033eaa*/
              v1734 = (wchar_t *)v1898; /*0x10033eb6*/
              v1272 = dword_103F4E4C; /*0x10033eb9*/
              if ( dword_103F4E4C != 0 ) /*0x10033ec0*/
              {
                if ( dword_103F4E4C == 1 ) /*0x10033ec9*/
                  goto LABEL_455; /*0x10033ec9*/
LABEL_585:
                if ( v1272 == 2 ) /*0x100353ba*/
                  goto LABEL_586; /*0x100353ba*/
LABEL_456:
                if ( v1272 != 3 ) /*0x10033f6b*/
                  goto LABEL_588; /*0x10033f6b*/
              }
              else
              {
                v1727 = *(_DWORD *)ArgList; /*0x10035333*/
                __asm { vmovss xmm0, dword ptr [esi+90h] } /*0x10035336*/
                __asm
                {
                  vcvtsi2ss xmm1, xmm3, eax
                  vsubss xmm0, xmm0, xmm1
                  vaddss xmm0, xmm0, ds:dword_10185850
                  vcvttss2si eax, xmm0
                }
                v1738 = _EAX; /*0x10035356*/
                __asm /*0x10035359*/
                {
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+7Ch]
                  vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                }
                v1918 = i; /*0x10035371*/
                __asm { vcvttss2si ebx, xmm0 } /*0x10035377*/
                sub_1017B960(a1: &v1897); /*0x10035388*/
                sub_10177250( /*0x100353a1*/
                  a1: v1727,
                  a2: (int)&v1897,
                  a3: _EBX,
                  a4: (int)v1738,
                  a5: (char)v1918,
                  a6: 8,
                  Format: v1725,
                  ArgList: v1724[0]);
                v1272 = dword_103F4E4C; /*0x100353a9*/
                if ( dword_103F4E4C != 1 ) /*0x100353b1*/
                  goto LABEL_585; /*0x100353b1*/
LABEL_455:
                __asm /*0x10033ecf*/
                {
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                  vaddss xmm0, xmm0, dword ptr [esi+90h]
                }
                __asm { vcvtsi2ss xmm1, xmm3, eax }
                v1727 = *(_DWORD *)ArgList; /*0x10033efc*/
                __asm /*0x10033eff*/
                {
                  vsubss xmm0, xmm0, xmm1
                  vcvttss2si eax, xmm0
                }
                v1738 = _EAX; /*0x10033f07*/
                v1917 = i; /*0x10033f0d*/
                __asm /*0x10033f13*/
                {
                  vmovss xmm0, dword ptr [esi+0C0h]
                  vaddss xmm0, xmm0, dword ptr [esi+7Ch]
                  vaddss xmm0, xmm0, ds:dword_1018531C
                  vcvttss2si ebx, xmm0
                }
                sub_1017B960(a1: &v1897); /*0x10033f39*/
                sub_10177250( /*0x10033f52*/
                  a1: v1727,
                  a2: (int)&v1897,
                  a3: _EBX,
                  a4: (int)v1738,
                  a5: (char)v1917,
                  a6: 1,
                  Format: v1725,
                  ArgList: v1724[0]);
                v1272 = dword_103F4E4C; /*0x10033f5a*/
                if ( dword_103F4E4C != 2 ) /*0x10033f62*/
                  goto LABEL_456; /*0x10033f62*/
LABEL_586:
                __asm /*0x100353c0*/
                {
                  vmovss xmm0, dword ptr [esi+90h]
                  vaddss xmm0, xmm0, dword ptr [esi+0A8h]
                }
                v1727 = *(_DWORD *)ArgList; /*0x100353d5*/
                __asm /*0x100353d8*/
                {
                  vaddss xmm0, xmm0, ds:dword_10185854
                  vcvttss2si eax, xmm0
                }
                v1738 = _EAX; /*0x100353e4*/
                __asm /*0x100353e7*/
                {
                  vmovss xmm0, ds:dword_10184EDC
                  vmulss xmm0, xmm0, dword ptr [esi+7Ch]
                  vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                }
                v1916 = i; /*0x100353ff*/
                __asm { vcvttss2si ebx, xmm0 } /*0x10035405*/
                sub_1017B960(a1: &v1897); /*0x10035416*/
                sub_10177250( /*0x1003542f*/
                  a1: v1727,
                  a2: (int)&v1897,
                  a3: _EBX,
                  a4: (int)v1738,
                  a5: (char)v1916,
                  a6: 8,
                  Format: v1725,
                  ArgList: v1724[0]);
                if ( dword_103F4E4C != 3 ) /*0x1003543f*/
                {
LABEL_588:
                  if ( byte_103F4DE5 == 1 ) /*0x100354da*/
                  {
                    if ( dword_103F4DEC == 1 ) /*0x100354e8*/
                    {
                      __asm { vcvttss2si eax, dword ptr [esi+0A8h] } /*0x10035604*/
                      v1725 = _EAX; /*0x1003560c*/
                      __asm { vcvttss2si edx, dword ptr [esi+7Ch] } /*0x1003560f*/
                      __asm { vcvttss2si edi, dword ptr [esi+90h] }
                      v1429 = (char)i; /*0x10035622*/
                      v1968 = i; /*0x10035625*/
LABEL_579:
                      __asm { vcvttss2si ebx, dword ptr [esi+0C0h] } /*0x100351ed*/
                      sub_101778E0(a1: _EBX, a2: _EDI, a3: _EDX, a4: v1725, a5: v1429, a6: 1); /*0x100351fe*/
                    }
                    else if ( dword_103F4DEC == 0 ) /*0x100354f0*/
                    {
                      __asm /*0x100354f6*/
                      {
                        vmovss xmm0, dword ptr [esi+0A8h]
                        vmovss dword ptr [esi+40h], xmm0
                        vcvttss2si eax, xmm0
                      }
                      v1729 = _EAX; /*0x10035507*/
                      __asm /*0x1003550a*/
                      {
                        vmovss xmm0, dword ptr [esi+7Ch]
                        vmovss dword ptr [esi+1Ch], xmm0
                        vcvttss2si edx, xmm0
                      }
                      __asm
                      {
                        vmovss xmm0, dword ptr [esi+90h]
                        vmovss dword ptr [esi+14h], xmm0
                        vcvttss2si ebx, xmm0
                      }
                      LOBYTE(_EAX) = (_BYTE)i; /*0x1003552f*/
                      v1969 = i; /*0x10035532*/
                      __asm /*0x10035538*/
                      {
                        vmovss xmm0, dword ptr [esi+0C0h]
                        vmovss dword ptr [esi+60h], xmm0
                        vcvttss2si edi, xmm0
                      }
                      sub_10177840(a1: _EDI, a2: _EBX, a3: _EDX, a4: v1729, a5: _EAX, a6: 1); /*0x10035552*/
                      __asm /*0x10035557*/
                      {
                        vmovss xmm1, ds:dword_10185854
                        vaddss xmm0, xmm1, dword ptr [esi+40h]
                        vcvttss2si eax, xmm0
                      }
                      i = _EAX; /*0x10035568*/
                      __asm /*0x1003556b*/
                      {
                        vaddss xmm0, xmm1, dword ptr [esi+1Ch]
                        vcvttss2si edx, xmm0
                      }
                      __asm
                      {
                        vmovss xmm1, ds:dword_1018555C
                        vaddss xmm0, xmm1, dword ptr [esi+14h]
                        vcvttss2si edi, xmm0
                      }
                      v1914 = -16777216; /*0x1003558b*/
                      __asm /*0x10035595*/
                      {
                        vaddss xmm0, xmm1, dword ptr [esi+60h]
                        vcvttss2si ebx, xmm0
                      }
                      sub_10177840(a1: _EBX, a2: _EDI, a3: _EDX, a4: _EAX, a5: 0, a6: 1); /*0x100355ad*/
                      __asm /*0x100355b2*/
                      {
                        vmovss xmm1, ds:dword_10185850
                        vaddss xmm0, xmm1, dword ptr [esi+40h]
                        vcvttss2si eax, xmm0
                      }
                      i = _EAX; /*0x100355c3*/
                      __asm /*0x100355c6*/
                      {
                        vaddss xmm0, xmm1, dword ptr [esi+1Ch]
                        vcvttss2si edx, xmm0
                      }
                      __asm
                      {
                        vmovss xmm1, ds:dword_10184184
                        vaddss xmm0, xmm1, dword ptr [esi+14h]
                        vcvttss2si edi, xmm0
                      }
                      v1913 = -16777216; /*0x100355e6*/
                      __asm /*0x100355f0*/
                      {
                        vaddss xmm0, xmm1, dword ptr [esi+60h]
                        vcvttss2si ebx, xmm0
                      }
LABEL_577:
                      sub_10177840(a1: _EBX, a2: _EDI, a3: _EDX, a4: i, a5: 0, a6: 1); /*0x100351b3*/
                    }
                  }
                  goto LABEL_431; /*0x100351c1*/
                }
              }
              v1727 = *(_DWORD *)ArgList; /*0x1003544a*/
              v1915 = i; /*0x10035450*/
              __asm /*0x10035456*/
              {
                vmovss xmm0, ds:dword_10184EDC
                vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                vaddss xmm0, xmm0, dword ptr [esi+90h]
              }
              __asm
              {
                vcvtsi2ss xmm1, xmm3, eax
                vsubss xmm0, xmm0, xmm1
                vcvttss2si eax, xmm0
              }
              v1734 = (wchar_t *)_EAX; /*0x10035486*/
              __asm /*0x10035489*/
              {
                vmovss xmm0, ds:dword_10185884
                vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                vaddss xmm0, xmm0, ds:dword_1018550C
                vcvttss2si ebx, xmm0
              }
              sub_1017B960(a1: &v1897); /*0x100354b2*/
              sub_10177250( /*0x100354cb*/
                a1: v1727,
                a2: (int)&v1897,
                a3: _EBX,
                a4: (int)v1734,
                a5: (char)v1915,
                a6: 1,
                Format: v1725,
                ArgList: v1724[0]);
              goto LABEL_588; /*0x100354cb*/
            }
            v1519 = sub_10168520(a1: v1271); /*0x10034e4b*/
            if ( v1519 == 0 ) /*0x10034e52*/
              goto LABEL_588; /*0x10034e52*/
            v1520 = v1519; /*0x10034e58*/
            v1521 = dword_103F4E4C; /*0x10034e5a*/
            if ( dword_103F4E4C != 0 ) /*0x10034e61*/
            {
              if ( dword_103F4E4C == 2 ) /*0x10034e6a*/
                goto LABEL_565; /*0x10034e6a*/
LABEL_581:
              if ( v1521 == 1 ) /*0x1003526c*/
                goto LABEL_582; /*0x1003526c*/
LABEL_566:
              if ( v1521 != 3 ) /*0x10034ed4*/
                goto LABEL_588; /*0x10034ed4*/
            }
            else
            {
              v1973 = i; /*0x10035211*/
              __asm /*0x10035217*/
              {
                vmovss xmm0, ds:dword_10185888
                vaddss xmm0, xmm0, dword ptr [esi+90h]
                vaddss xmm0, xmm0, ds:dword_10185850
                vcvttss2si eax, xmm0
                vmovss xmm0, ds:dword_10184EDC
                vmulss xmm0, xmm0, dword ptr [esi+7Ch]
                vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                vcvttss2si edx, xmm0
              }
              sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v1520, a6: 8, a7: (char)i); /*0x10035256*/
              v1521 = dword_103F4E4C; /*0x1003525b*/
              if ( dword_103F4E4C != 2 ) /*0x10035263*/
                goto LABEL_581; /*0x10035263*/
LABEL_565:
              v1972 = i; /*0x10034e70*/
              __asm /*0x10034e7f*/
              {
                vmovss xmm0, dword ptr [esi+90h]
                vaddss xmm0, xmm0, dword ptr [esi+0A8h]
                vaddss xmm0, xmm0, ds:dword_10185854
                vcvttss2si eax, xmm0
                vmovss xmm0, ds:dword_10184EDC
                vmulss xmm0, xmm0, dword ptr [esi+7Ch]
                vaddss xmm0, xmm0, dword ptr [esi+0C0h]
                vcvttss2si edx, xmm0
              }
              sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v1520, a6: 8, a7: (char)i); /*0x10034ebe*/
              v1521 = dword_103F4E4C; /*0x10034ec3*/
              if ( dword_103F4E4C != 1 ) /*0x10034ecb*/
                goto LABEL_566; /*0x10034ecb*/
LABEL_582:
              __asm /*0x10035272*/
              {
                vmovss xmm0, ds:dword_10184EDC
                vmulss xmm0, xmm0, dword ptr [esi+0A8h]
                vaddss xmm0, xmm0, dword ptr [esi+90h]
                vcvttss2si eax, xmm0
              }
              __asm
              {
                vmovss xmm0, dword ptr [esi+0C0h]
                vaddss xmm0, xmm0, dword ptr [esi+7Ch]
              }
              v1971 = i; /*0x100352a4*/
              __asm /*0x100352aa*/
              {
                vaddss xmm0, xmm0, ds:dword_10185854
                vcvttss2si edx, xmm0
              }
              sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v1520, a6: 16, a7: (char)i); /*0x100352c0*/
              if ( dword_103F4E4C != 3 ) /*0x100352cd*/
                goto LABEL_588; /*0x100352cd*/
            }
            __asm /*0x100352d9*/
            {
              vmovss xmm0, ds:dword_10184EDC
              vmulss xmm0, xmm0, dword ptr [esi+0A8h]
              vaddss xmm0, xmm0, dword ptr [esi+90h]
              vcvttss2si eax, xmm0
              vmovss xmm0, ds:dword_10185888
              vaddss xmm0, xmm0, dword ptr [esi+0C0h]
            }
            v1970 = i; /*0x10035308*/
            __asm /*0x1003530e*/
            {
              vaddss xmm0, xmm0, ds:dword_10185850
              vcvttss2si edx, xmm0
            }
            sub_10178600(a1: _EDX, a2: _EAX, a3: 50, a4: 50, a5: v1520, a6: 16, a7: (char)i); /*0x10035324*/
            goto LABEL_588; /*0x10035329*/
          default:
            goto LABEL_431;
        }
      }
    }
  }
LABEL_593:
  v1659 = *(int (__stdcall **)(_DWORD))(*(_DWORD *)dword_103EC728 + 420); /*0x10035630*/
  v2004 = -1; /*0x1003563e*/
  __asm /*0x1003564b*/
  {
    vmovss xmm0, dword ptr [esi+284h]
    vmovss [esp+450h+var_450], xmm0
  }
  return v1659(a1: *(_DWORD *)&v1723[16]); /*0x1003565a*/
}