//
//  QrCodeTests.m
//  QrCodeTests
//
//  Created by Pai Peng on 2022/2/20.
//

#import <XCTest/XCTest.h>
#include "encoder.h"
#include "decoder.h"


@interface QrCodeTests : XCTestCase

@end

@implementation QrCodeTests

- (void)setUp {
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
}

- (void)testEncode {
    scanner_t scanner;
    scanner.c = 0;
    scanner.verbosity = 0;
    
    qrc_encode(&scanner, "TEST01234");
    
    for (int i = 0; i < scanner.s; i++) {
        for (int j = 0; j < scanner.s; j++) {
            printf("%02X ", scanner.d[i*scanner.s+j] * 0xFF);
        }
        printf("\n");
    }
    free(scanner.d);
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
