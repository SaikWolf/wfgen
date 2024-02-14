/*
 * Copyright (c) 2007 - 2015 Joseph Gaeddert
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

//
// autotest nco sincos data for f=0.707106781187
//

#include <complex.h>

float complex nco_sincos_fsqrt1_2[256] = {
    1.000000000000 + _Complex_I*  0.000000000000,
    0.760244597076 + _Complex_I*  0.649636939080,
    0.155943694765 + _Complex_I*  0.987765945993,
   -0.523133894289 + _Complex_I*  0.852250508152,
   -0.951363128126 + _Complex_I*  0.308071742363,
   -0.923403461740 + _Complex_I* -0.383830752866,
   -0.452661857292 + _Complex_I* -0.891682254479,
    0.235135999123 + _Complex_I* -0.971962479686,
    0.810183603115 + _Complex_I* -0.586176193003,
    0.996739414692 + _Complex_I*  0.080687912355,
    0.705347906308 + _Complex_I*  0.708861291838,
    0.075734454968 + _Complex_I*  0.997128022037,
   -0.590194485905 + _Complex_I*  0.807261090854,
   -0.973118793234 + _Complex_I*  0.230303743466,
   -0.889422123833 + _Complex_I* -0.457086737542,
   -0.379237935093 + _Complex_I* -0.925299188688,
    0.312794941512 + _Complex_I* -0.949820680215,
    0.854839263647 + _Complex_I* -0.518892891961,
    0.986978921600 + _Complex_I*  0.160849645067,
    0.645851521500 + _Complex_I*  0.763463039168,
   -0.004968662133 + _Complex_I*  0.999987656122,
   -0.653406318582 + _Complex_I*  0.757007386250,
   -0.988528584662 + _Complex_I*  0.151033894564,
   -0.849640712505 + _Complex_I* -0.527361981615,
   -0.303340937614 + _Complex_I* -0.952882089016,
    0.388414094720 + _Complex_I* -0.921484938034,
    0.893920371491 + _Complex_I* -0.448225801838,
    0.970782170565 + _Complex_I*  0.239962449800,
    0.582143428727 + _Complex_I*  0.813086113761,
   -0.085639377739 + _Complex_I*  0.996326200087,
   -0.712357177173 + _Complex_I*  0.701817107322,
   -0.997492012529 + _Complex_I*  0.070779127866,
   -0.804318649129 + _Complex_I* -0.594198208230,
   -0.225465802126 + _Complex_I* -0.974251082664,
    0.461500333346 + _Complex_I* -0.887140035351,
    0.927172072075 + _Complex_I* -0.374635754786,
    0.948254783364 + _Complex_I*  0.317510418456,
    0.514639079331 + _Complex_I*  0.857406915079,
   -0.165751624352 + _Complex_I*  0.986167530912,
   -0.766662633072 + _Complex_I*  0.642050159295,
   -0.999950624793 + _Complex_I* -0.009937201600,
   -0.753751486611 + _Complex_I* -0.657159566948,
   -0.146120365674 + _Complex_I* -0.989266818778,
    0.531577049558 + _Complex_I* -0.847009941136,
    0.954377525386 + _Complex_I* -0.298602644058,
    0.919543664932 + _Complex_I*  0.392987847501,
    0.443778680694 + _Complex_I*  0.896136419616,
   -0.244782976343 + _Complex_I*  0.969577895010,
   -0.815968551135 + _Complex_I*  0.578096292635,
   -0.995888388425 + _Complex_I* -0.090588728879,
   -0.698268982046 + _Complex_I* -0.715835476008,
   -0.065822053387 + _Complex_I* -0.997831377181,
    0.598187261134 + _Complex_I* -0.801356350581,
    0.975359320021 + _Complex_I* -0.220622294542,
    0.884836045372 + _Complex_I*  0.465902535742,
    0.370024325563 + _Complex_I*  0.929022065665,
   -0.322218056780 + _Complex_I*  0.946665476229,
   -0.859953399058 + _Complex_I*  0.510372561418,
   -0.985331793961 + _Complex_I* -0.170649511602,
   -0.638232946313 + _Complex_I* -0.769843299796,
    0.014905495740 + _Complex_I* -0.999888906928,
    0.660896591520 + _Complex_I* -0.750476978539,
    0.989980630117 + _Complex_I* -0.141203229401,
    0.844358258992 + _Complex_I*  0.535778994056,
    0.293856978673 + _Complex_I*  0.955849400316,
   -0.397551898294 + _Complex_I*  0.917579690361,
   -0.898330344143 + _Complex_I*  0.439320603650,
   -0.968349682754 + _Complex_I* -0.249597459743,
   -0.574034884644 + _Complex_I* -0.818830844077,
    0.095535843587 + _Complex_I* -0.995425990514,
    0.719296102472 + _Complex_I* -0.694703618076,
    0.998146107617 + _Complex_I* -0.060863353909,
    0.798374268344 + _Complex_I*  0.602161546138,
    0.215773340288 + _Complex_I*  0.976443477945,
   -0.470293236050 + _Complex_I*  0.882510210777,
   -0.930849123784 + _Complex_I*  0.365403761270,
   -0.945052798049 + _Complex_I* -0.326917740264,
   -0.506093443552 + _Complex_I* -0.862478652718,
    0.175543185898 + _Complex_I* -0.984471731379,
    0.773004960816 + _Complex_I* -0.634399976792,
    0.999802504049 + _Complex_I*  0.019873421897,
    0.747183942875 + _Complex_I*  0.664617300038,
    0.136282607136 + _Complex_I*  0.990670001056,
   -0.539967711374 + _Complex_I*  0.841685731538,
   -0.957297677470 + _Complex_I*  0.289104058618,
   -0.915593062806 + _Complex_I* -0.402106134423,
   -0.434851680767 + _Complex_I* -0.900502090911,
    0.254405781142 + _Complex_I* -0.967097564117,
    0.821672921923 + _Complex_I* -0.569959305019,
    0.994939017768 + _Complex_I*  0.100480599729,
    0.691121103433 + _Complex_I*  0.722738971129,
    0.055903151852 + _Complex_I*  0.998436196065,
   -0.606120965124 + _Complex_I*  0.795372476037,
   -0.977503529671 + _Complex_I*  0.210919059075,
   -0.880162588986 + _Complex_I* -0.474672325874,
   -0.360774175978 + _Complex_I* -0.932653201328,
    0.331609352882 + _Complex_I* -0.943416788636,
    0.864982613715 + _Complex_I* -0.501801831374,
    0.983587364400 + _Complex_I*  0.180432526426,
    0.630551345359 + _Complex_I*  0.776147538079,
   -0.024840857424 + _Complex_I*  0.999691418290,
   -0.668321600646 + _Complex_I*  0.743872460917,
   -0.991334914576 + _Complex_I*  0.131358620360,
   -0.838992424751 + _Complex_I* -0.544143098100,
   -0.284344001234 + _Complex_I* -0.958722321093,
    0.406650443454 + _Complex_I* -0.913583831314,
    0.902651606302 + _Complex_I* -0.430372022371,
    0.965821570012 + _Complex_I*  0.259207821834,
    0.565869654379 + _Complex_I*  0.824494714509,
   -0.105422875231 + _Complex_I*  0.994427482212,
   -0.726163996983 + _Complex_I*  0.687521526561,
   -0.998701635364 + _Complex_I*  0.050941569671,
   -0.792351047769 + _Complex_I* -0.610065420344,
   -0.206059570743 + _Complex_I* -0.978539449029,
    0.479039697103 + _Complex_I* -0.877793237955,
    0.934434253757 + _Complex_I* -0.356135683981,
    0.941757488380 + _Complex_I*  0.336292778810,
    0.497497830836 + _Complex_I*  0.867465220233,
   -0.185317412481 + _Complex_I*  0.982678714856,
   -0.779270954001 + _Complex_I*  0.626687147028,
   -0.999555652393 + _Complex_I* -0.029807679686,
   -0.740542614416 + _Complex_I* -0.672009401894,
   -0.126431390635 + _Complex_I* -0.991975354261,
    0.548305051154 + _Complex_I* -0.836278405125,
    0.960123296014 + _Complex_I* -0.279576924033,
    0.911552045487 + _Complex_I*  0.411184713198,
    0.425881739056 + _Complex_I*  0.904778837252,
   -0.264003463266 + _Complex_I*  0.964521731940,
   -0.827296152171 + _Complex_I*  0.561766033686,
   -0.993891396472 + _Complex_I* -0.110362548078,
   -0.683904976326 + _Complex_I* -0.729571095478,
   -0.045978729857 + _Complex_I* -0.998942418962,
    0.613994814417 + _Complex_I* -0.789310058132,
    0.979551210443 + _Complex_I* -0.201194995263,
    0.875402216180 + _Complex_I*  0.483395241917,
    0.351488399794 + _Complex_I*  0.936192237102,
   -0.340967902424 + _Complex_I*  0.940074938245,
   -0.869926410981 + _Complex_I*  0.493181548192,
   -0.981745805180 + _Complex_I* -0.190197723464,
   -0.622807477199 + _Complex_I* -0.782375131472,
    0.034773766063 + _Complex_I* -0.999395209711,
    0.675680612738 + _Complex_I* -0.737194485581,
    0.992591304302 + _Complex_I* -0.121501039602,
    0.833543739662 + _Complex_I*  0.552453467788,
    0.274802944706 + _Complex_I*  0.961500567644,
   -0.415708831715 + _Complex_I*  0.909497755486,
   -0.906883731242 + _Complex_I*  0.421380941677,
   -0.963198081990 + _Complex_I* -0.268792587045,
   -0.557648544251 + _Complex_I* -0.830077165747,
    0.115299496322 + _Complex_I* -0.993330773785,
    0.732960182500 + _Complex_I* -0.680271542011,
    0.999158540912 + _Complex_I* -0.041014754931,
    0.786249582201 + _Complex_I*  0.617909050337,
    0.196325452731 + _Complex_I*  0.980538788937,
   -0.487738852787 + _Complex_I*  0.872989582688,
   -0.937927107962 + _Complex_I*  0.346832438147,
   -0.938369179770 + _Complex_I* -0.345634608305,
   -0.488853090003 + _Complex_I* -0.872366125199,
    0.195073338893 + _Complex_I* -0.980788658404,
    0.785459993857 + _Complex_I* -0.618912431650,
    0.999210094204 + _Complex_I*  0.039738993954,
    0.733828157068 + _Complex_I*  0.679335142543,
    0.116567688981 + _Complex_I*  0.993182749491,
   -0.556588245585 + _Complex_I*  0.830788495874,
   -0.962854101984 + _Complex_I*  0.270022181112,
   -0.907421012027 + _Complex_I* -0.420222687313,
   -0.416869741348 + _Complex_I* -0.908966236308,
    0.273575074938 + _Complex_I* -0.961850652842,
    0.832837686581 + _Complex_I* -0.553517287725,
    0.992745627990 + _Complex_I*  0.120233598080,
    0.676621313319 + _Complex_I*  0.736331174379,
    0.036049767444 + _Complex_I*  0.999349995881,
   -0.621808031469 + _Complex_I*  0.783169695533,
   -0.981502160129 + _Complex_I*  0.191451063363,
   -0.870555397043 + _Complex_I* -0.492070422480,
   -0.342167913985 + _Complex_I* -0.939638823506,
    0.350292781243 + _Complex_I* -0.936640255065,
    0.874784302655 + _Complex_I* -0.484512563128,
    0.979807298156 + _Complex_I*  0.199944138398,
    0.615002106542 + _Complex_I*  0.788525464997,
   -0.044703240778 + _Complex_I*  0.999000310442,
   -0.682972901089 + _Complex_I*  0.730443711985,
   -0.993749675226 + _Complex_I*  0.111631460566,
   -0.828012741783 + _Complex_I* -0.560709282467,
   -0.265234751275 + _Complex_I* -0.964183865617,
    0.424726168556 + _Complex_I* -0.905321866379,
    0.911026301038 + _Complex_I* -0.412348249441,
    0.960479477760 + _Complex_I*  0.278350808877,
    0.549372366100 + _Complex_I*  0.835577646521,
   -0.125164731540 + _Complex_I*  0.992135973533,
   -0.739683987895 + _Complex_I*  0.672954380365,
   -0.999516779141 + _Complex_I*  0.031083889968,
   -0.780070474162 + _Complex_I* -0.625691661556,
   -0.186571947499 + _Complex_I* -0.982441300235,
    0.496389844059 + _Complex_I* -0.868099719338,
    0.941327341476 + _Complex_I* -0.337494942465,
    0.934888206815 + _Complex_I*  0.354942306239,
    0.480160074725 + _Complex_I*  0.877180883649,
   -0.204810001732 + _Complex_I*  0.978801748665,
   -0.791571469213 + _Complex_I*  0.611076598413,
   -0.998765863605 + _Complex_I* -0.049666383980,
   -0.727041233885 + _Complex_I* -0.686593798567,
   -0.106692476220 + _Complex_I* -0.994292067512,
    0.564816476695 + _Complex_I* -0.825216545916,
    0.965489825714 + _Complex_I* -0.260440773388,
    0.903200370366 + _Complex_I*  0.429219164264,
    0.407816577581 + _Complex_I*  0.913063874573,
   -0.283119670959 + _Complex_I*  0.959084590595,
   -0.838296977925 + _Complex_I*  0.545213881703,
   -0.991501825466 + _Complex_I* -0.130092774964,
   -0.669270833677 + _Complex_I* -0.743018540273,
   -0.026117245101 + _Complex_I* -0.999658886575,
    0.629559844720 + _Complex_I* -0.776951994602,
    0.983356186070 + _Complex_I* -0.181688225591,
    0.865622610200 + _Complex_I*  0.500697010886,
    0.332813638953 + _Complex_I*  0.942992620187,
   -0.359583068506 + _Complex_I*  0.933113078273,
   -0.879555809016 + _Complex_I*  0.475795732248,
   -0.977772034756 + _Complex_I* -0.209670808766,
   -0.607136004174 + _Complex_I* -0.794597931306,
    0.054628301030 + _Complex_I* -0.998506759480,
    0.690197745585 + _Complex_I* -0.723620806770,
    0.994809912959 + _Complex_I* -0.101750857877,
    0.822399977304 + _Complex_I*  0.568909726873,
    0.255640365802 + _Complex_I*  0.966771950034,
   -0.433701563513 + _Complex_I*  0.901056576363,
   -0.915078906610 + _Complex_I*  0.403274837644,
   -0.957666025784 + _Complex_I* -0.287881543451,
   -0.541041937199 + _Complex_I* -0.840995613658,
    0.135017606689 + _Complex_I* -0.990843199444,
    0.746334749190 + _Complex_I* -0.665570764195,
    0.999776314675 + _Complex_I* -0.021149955457,
    0.773814333841 + _Complex_I*  0.633412485465,
    0.176800018209 + _Complex_I*  0.984246795047,
   -0.504991816627 + _Complex_I*  0.863124130783,
   -0.944634618526 + _Complex_I*  0.328124119019,
   -0.931314913263 + _Complex_I* -0.364214953474,
   -0.471419643442 + _Complex_I* -0.881909020125,
    0.214526439499 + _Complex_I* -0.976718181850,
    0.797604776560 + _Complex_I* -0.603180421109,
    0.998223004463 + _Complex_I*  0.059588869430,
    0.720182515080 + _Complex_I*  0.693784653169,
    0.096806727533 + _Complex_I*  0.995303198781,
   -0.572988931946 + _Complex_I*  0.819563105482,
   -0.968030206924 + _Complex_I*  0.250833647030,
   -0.898890537295 + _Complex_I* -0.438173255644,
   -0.398723141757 + _Complex_I* -0.917071347403,
    0.292636308795 + _Complex_I* -0.956223818347,
    0.843673487096 + _Complex_I* -0.536856635585,
    0.990160111727 + _Complex_I*  0.139939105133,
    0.661854263264 + _Complex_I*  0.749632532778,
    0.016182143669 + _Complex_I*  0.999869060541,
   -0.637249488677 + _Complex_I*  0.770657569340,
   -0.985113105181 + _Complex_I*  0.171907446032,
   -0.860604342767 + _Complex_I* -0.509274155254,
   -0.323426498437 + _Complex_I* -0.946253295957};

