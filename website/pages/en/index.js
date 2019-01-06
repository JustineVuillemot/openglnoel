/**
 * Copyright (c) 2017-present, Facebook, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

const React = require('react');

const CompLibrary = require('../../core/CompLibrary.js');

const MarkdownBlock = CompLibrary.MarkdownBlock; /* Used to read markdown */
const Container = CompLibrary.Container;
const GridBlock = CompLibrary.GridBlock;

class HomeSplash extends React.Component {
    render() {
        const { siteConfig, language = '' } = this.props;
        const { baseUrl, docsUrl } = siteConfig;
        const docsPart = `${docsUrl ? `${docsUrl}/` : ''}`;
        const langPart = `${language ? `${language}/` : ''}`;
        const docUrl = doc => `${baseUrl}${docsPart}${langPart}${doc}`;

        const SplashContainer = props => (
            <div className="homeContainer">
                <div className="homeSplashFade">
                    <div className="wrapper homeWrapper">{props.children}</div>
                </div>
            </div>
        );

        const Logo = props => (
            <div className="projectLogo">
                <img src={props.img_src} alt="Project Logo" />
            </div>
        );

        const ProjectTitle = () => (
            <h2 className="projectTitle">
                {siteConfig.title}
                <small>{siteConfig.tagline}</small>
            </h2>
        );

        const PromoSection = props => (
            <div className="section promoSection">
                <div className="promoRow">
                    <div className="pluginRowBlock">{props.children}</div>
                </div>
            </div>
        );

        const Button = props => (
            <div className="pluginWrapper buttonWrapper">
                <a className="button" href={props.href} target={props.target}>
                    {props.children}
                </a>
            </div>
        );

        return (
            <SplashContainer>
                <Logo img_src={`${baseUrl}img/logo.png`} />
                <div className="inner">
                    <ProjectTitle siteConfig={siteConfig} />
                </div>
            </SplashContainer>
        );
    }
}

class Index extends React.Component {
    render() {
        const { config: siteConfig, language = '' } = this.props;
        const { baseUrl } = siteConfig;

        const Block = props => (
            <Container
                padding={['bottom', 'top']}
                id={props.id}
                background={props.background}>
                <GridBlock
                    align="center"
                    contents={props.children}
                    layout={props.layout}
                />
            </Container>
        );

        const Features = () => (
            <Block layout="fourColumn">
                {[
                    {
                        content: 'Installation du SDK des TDs pour commencer à coder.',
                        // image: `${baseUrl}img/logo.png`,
                        imageAlign: 'top',
                        title: 'Introduction et SDK',
                    },
                    {
                        content: "Les bases d'OpenGL 3+: VBOs, VAOs, shaders, textures, etc. (Niveau Imac 2 et Master 1).",
                        // image: `${baseUrl}img/logo.png`,
                        imageAlign: 'top',
                        title: 'Bronze Level',
                    },
                    {
                        content: 'Techniques avancées: Deferred rendering, shadow mapping, compute shaders, PBR, etc. (Niveau Imac 3 et Master 2).',
                        // image: `${baseUrl}img/logo.png`,
                        imageAlign: 'top',
                        title: 'Silver Level',
                    },
                ]}
            </Block>
        );

        return (
            <div>
                <HomeSplash siteConfig={siteConfig} language={language} />
                <div>
                    <Features />
                </div>
            </div>
        );
    }
}

module.exports = Index;
